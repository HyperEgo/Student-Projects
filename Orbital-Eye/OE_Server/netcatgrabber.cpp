#include "netcatgrabber.h"

/**
  * Class NetCatGrabber.cpp
  *
  * CPE 453, Team 1
  * Written by:  John Jetton
  *
  * This class connects to the space-track.org website and downloads a
  * subset of the three-line element sets of the space catalog.
  *
  * Inputs:
  *     filename(QString):  the path and filename to save the downloaded catalog
  *     username(QString):  valid username for the website
  *     password(QString):  valid password for the website
  *     catalog size(int):  number of entries to download
  *
  * Outputs:
  *     file:  The data downloaded is saved to the file given.
  *
  * Notes:
  *     There are many qDebug statements right now; they will be deleted for demo.
  *     Currently just downloads the requested number of satellites from the
  *     beginning of the catalog.  Since they've been up there so long, the early
  *     ones tend to be deep-space objects.
  *     Networking is threaded, so no real indication right now of when the
  *     download is finished other than a qDebug "done".  This needs to be tied
  *     to an indicator for the user.
  */


/*
 *  Constructor
 *
 *  args: the calling QWidget
 */
NetCatGrabber::NetCatGrabber(QWidget *parent) {
    aTimer = new QTimer(parent);
    connect(aTimer, SIGNAL(timeout()), this, SLOT(update()));
    nam = new QNetworkAccessManager();
}

/*
 *  Function setCatFilePath
 *  args:  QString filenameAndPath
 *
 *  Opens a "rw" file at the given location.  Tests that it is open and writable.
 *  If the file contains data, this function deletes that data.
 */
void NetCatGrabber::setCatFilePath(QString filenameAndPath) {
    saveLocation = filenameAndPath;
    theCatalogFile = new QFile(saveLocation);
    theCatalogFile->open(QIODevice::ReadWrite);
    if (!theCatalogFile->isOpen()) {
        QMessageBox msgBox;
        msgBox.setText("Catalog file cannot be opened!");
        msgBox.exec();
    }
    if (!theCatalogFile->isWritable()) {
        QMessageBox msgBox;
        msgBox.setText("Catalog file is not writable!");
        msgBox.exec();
    }
    if (theCatalogFile->size() > 0) {
        // If there is old data in the file, we must erase it first
        theCatalogFile->resize(0);
    }
}

/*
 *  Function setUserName
 *  args:  QString username
 *  return:  void
 *
 *  Not currently checked for validity.
 */
void NetCatGrabber::setUserName(QString name) {
    userName = name;
}

/*
 *  Function setUserPass
 *  args:  QString user password
 *  return:  void
 *
 *  Not currently checked for validity.
 */
void NetCatGrabber::setUserPass(QString pass) {
    userPass = pass;
}


/*
 *  Function setCatalogSize
 *  args:  int size
 *  return:  void
 *
 *  The number of satellite entries to download from the website.
 *  Not currently checked for validity.
 */
void NetCatGrabber::setCatalogSize(int size) {
    numCatalogEntries = size;
}

/*
 *  Function buildQuery
 *  args:  none
 *  return:  bool indicating success or failure
 *
 *  Private function called from getCatalog().  Validity checks
 *  are limited to verifying that the username, password, and
 *  catalog size are not empty.
 */
bool NetCatGrabber::buildQuery() {
    // Verify entities in the network query
    if (userName.isNull() || userName.isEmpty()) return false;
    if (userPass.isNull() || userPass.isEmpty()) return false;
    if (numCatalogEntries < -1) return false;

    // Build the query
    // The website
    QString CATALOG_SITE = "https://www.space-track.org/";
    // QByteArray contains the final query
    theQuery.append("identity=");
    theQuery.append(userName);
    theQuery.append("&password=");
    theQuery.append(userPass);
    theQuery.append("&query=");
    theQuery.append(CATALOG_SITE);
    theQuery.append("basicspacedata/query/class/tle_latest/ORDINAL/1/EPOCH/>now-30/orderby/NORAD_CAT_ID/");
    // take out the limit if number is -1
    if (numCatalogEntries == -1) {
        // get them all - don't append a limit
    } else {
        // append a limit
        theQuery.append("limit/");
        theQuery.append(QByteArray::number(numCatalogEntries, 10));
    }
    theQuery.append("/format/3le");
    return true;
}

/*
 *  Function getCatalog
 *  args:  none
 *  return:  bool indicating success or failure of buildQuery function
 *
 *  Creates a network request using the username, password, and number of satellites.
 *  The network request is asynchronous - once passed this function returns regardless
 *  of success, failure, or completion.
 */
bool NetCatGrabber::getCatalog() {
    if (!buildQuery()) return false;
    // Start the timer
    aTimer->start(60000);
    // Instantiate QNetworkRequest object
    request = new QNetworkRequest();
    // Give it a header
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    // Set the logon URL for the QNetworkRequest
    QString LOGON_SITE = "https://www.space-track.org/ajaxauth/login";
    request->setUrl(QUrl(LOGON_SITE));
    // Post
    reply = nam->post(*request, theQuery);
    connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedReport(QNetworkReply*)));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(monitorProgress(qint64,qint64)));
    return true;
}

/*
 *  Function finishedReport
 *  args:  the QNetworkReply
 *  return:  void
 *
 *  Connected to the QNetworkAccessManager finished() signal.
 *  If successful, writes the requested data to file.
 *  This function takes time, it would be useful to connect it to a status
 *  indicator for the user.
 */
void NetCatGrabber::finishedReport(QNetworkReply *reply) {
    emit finishedDownload();
    reply->deleteLater();
    if (reply->error() == QNetworkReply::NoError) {
        // Success?
        QByteArray temp = reply->readAll();
        if (temp.contains("Failed")) {
            emit finishedDownload();
            QMessageBox msgBox;
            msgBox.setWindowTitle("Download Status");
            msgBox.setText("Login Failed!  Check username and password.");
            msgBox.exec();
            theCatalogFile->flush();
            theCatalogFile->close();
            reply->manager()->deleteLater();
            // Timer no longer needed
            aTimer->stop();
            return;
        } else {
            theCatalogFile->write(temp, temp.length());
            theCatalogFile->flush();
            theCatalogFile->close();
            reply->manager()->deleteLater();
            emit finishedDownload();
            // Timer no longer needed
            aTimer->stop();
            QMessageBox msgBox;
            msgBox.setWindowTitle("Download Status");
            msgBox.setText("Download Successful!");
            msgBox.exec();
        }
    } else {
        // Failure
        QString err = reply->errorString();
        if (err.contains("Host")) {
            emit finishedDownload();
            QMessageBox msgBox;
            msgBox.setWindowTitle("Download Status");
            msgBox.setText("Host Not Found!  Retry or select fallback catalog.");
            msgBox.exec();
            theCatalogFile->flush();
            theCatalogFile->close();
            reply->manager()->deleteLater();
            // Timer no longer needed
            aTimer->stop();
            return;
        }
//        int v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    }
    reply->manager()->deleteLater();
    emit finishedDownload();
}

void NetCatGrabber::monitorProgress(qint64 recieved, qint64 total) {
    double percent = (recieved/(double)total)*100.;
    emit percentFinished((int)percent);
}

void NetCatGrabber::update() {
    emit finishedDownload();
    aTimer->stop();
    QMessageBox msgBox;
    msgBox.setText("Network Interrupted!  Retry or select fallback catalog.");
    msgBox.exec();

}

/*
 *  Function provideAuthentication
 *  args:  none
 *  return:  void
 *
 *  Connected to the QNetworkAccessManager provideAuthentication() signal.
 *
 */
void NetCatGrabber::provideAuthentication(QNetworkReply *reply, QAuthenticator *authenticator) {
    qDebug() << "provideAuthentication called";
}

/*
 *  Function sslErrors
 *  args:  none
 *  return:  void
 *
 *  Connected to the QNetworkAccessManager sslErrors() signal.
 *
 */
void NetCatGrabber::sslErrors(QNetworkReply *reply, const QList<QSslError> &errors ) {
    qDebug() << "sslErrors called";
}
