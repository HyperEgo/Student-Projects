#ifndef NETCATGRABBER_H
#define NETCATGRABBER_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QSslConfiguration>
#include <QVariant>
#include <QSslSocket>
#include <QList>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QProgressDialog>

class NetCatGrabber : public QObject
{
    Q_OBJECT

public:
    NetCatGrabber(QWidget *parent = 0);
    void setCatFilePath(QString filenameAndPath);
    void setUserName(QString name);
    void setUserPass(QString pass);
    void setCatalogSize(int size);
    bool getCatalog();

signals:
    void finishedDownload();
    void percentFinished(int percent);


private:
    QString saveLocation;
    QFile *theCatalogFile;
    bool buildQuery();
    QString status;
    QNetworkAccessManager *nam;
    QNetworkRequest *request;
    QNetworkReply *reply;
    QString userName;
    QString userPass;
    int numCatalogEntries;
    QByteArray theQuery;
    QTimer *aTimer;

private slots:
    void finishedReport(QNetworkReply *reply);
    void provideAuthentication(QNetworkReply *reply, QAuthenticator *authenticator);
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors );
    void monitorProgress(qint64 recieved, qint64 total);
    void update();


};

#endif // NETCATGRABBER_H
