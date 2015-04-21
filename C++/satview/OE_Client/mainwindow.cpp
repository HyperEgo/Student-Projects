#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    ad = new AnimateDialog(this);

    // center main window on screen at execution
    QRect position = this->frameGeometry();
    position.moveCenter(QDesktopWidget().availableGeometry().center());
    move(position.topLeft());

    // set fixed window size
    QSize s = this->size();
    this->statusBar()->setSizeGripEnabled(false);
    this->setFixedSize(s);
    //    qDebug()<<"main window height = "<<this->height();
    //    qDebug()<<"main window width = "<<this->width();

    // Add logo and previews
    ui->logoLabel->setPixmap(QPixmap(":/images/oe_client_logo.jpg").scaled(ui->logoLabel->size()));
    ui->mapLabel->setPixmap(QPixmap(":/images/usa_mercator_map_no_degrees.png").scaled(ui->mapLabel->size(),Qt::KeepAspectRatioByExpanding));
    ui->instrumentLabel->setPixmap(QPixmap(":/images/FTDX3000-scope.jpg").scaled(ui->instrumentLabel->size(),Qt::KeepAspectRatioByExpanding));

    // Set label default values
    //    ui->userMapValue->setText(QString::number(0));
    ui->userAzimuth->setText(QString::number(0));
    ui->userElevation->setText(QString::number(0));

    // Disable buttons at start
    ui->statusBar->showMessage(">> Download Table");
    ui->tableButton->setDisabled(false);
    ui->locationButton->setDisabled(true);
    ui->calibrateButton->setDisabled(true);

    ui->animateButton->setDisabled(true);
    //    ui->animateButton->setDisabled(false);  // testing only


    //connect stmts
    connect(ui->locationButton, SIGNAL(clicked()), this, SLOT(getLocation()));
    connect(ui->calibrateButton, SIGNAL(clicked()), this, SLOT(getBoresight()));
    connect(ui->animateButton, SIGNAL(clicked()), this, SLOT(getAnimate()));
    connect(ui->actionQuit_3, SIGNAL(triggered()), this, SLOT(message()));
    connect(ui->tableButton, SIGNAL(clicked()), this, SLOT(getTable()));
    // connect(ad, SIGNAL(sendQuit()), this, SLOT(close()));
//    connect(ad, SIGNAL(sendQuit()), this, SLOT(message()));
    // connect(ui->actionQuit_3, SIGNAL(triggered()), SLOT(close()));

    connect(ui->actionHelp_2, SIGNAL(triggered()), this, SLOT(helpInfo()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(aboutInfo()));
}



void MainWindow::message()
{
    MainWindow dialog;

    if(QMessageBox::Ok == msgBox.information(&dialog, "Quit","Do you want to quit?",QMessageBox::Ok | QMessageBox::Cancel))
    {
        close();
        qDebug()<< "Application closed!";
    }

}


void MainWindow::helpInfo()
{
    MainWindow dialog1;

    help_msgBox.information(&dialog1,"Orbital Eye Help","Orital Eye Help: To run the program start with Get Table and go down in order to get to the Animation button.\n \n"
                            "Get Table Button: This is used to download the table from MySQL database to get information about each satellite.\n \n"
                            "Locale Button: Uses a mercator map to select location that is converted to x, y coordinates and prints the Lat, Long coordinates on the MainWindow. \n \n"
                            "Set Value Button: Select data from Azimuth(N,S,E,W) dial 0-360 degrees and Elevation slider 0-90 0 being horizon and 90 being stright over head. \n \n"
                            "Start Animation: Takes the Satellite Data, Map Data (Lat & Long), Azmiuth, Elevation and uses it to animate the satellites on the screen.",
                            QMessageBox::Close|QMessageBox::NoButton);


}


void MainWindow::aboutInfo()
{
    MainWindow dialog2;

    about_msgBox.information(&dialog2,"About Orbital Eye","Orbital Eye \n \n"
                             "This program was created for a class project. \n \n"
                             "Created by: Cindy Dawson, John Jetton, Fleance Collins" ,QMessageBox::Close|QMessageBox::NoButton);
}


/*********************************************************************
Function gets model view table from SQL database.
**********************************************************************/
void MainWindow::getTable() {

    // get SQL data
    // Connect to client database
    if(!connectToDatabase()) return;
    // Pull the data and populate our satList
    QSqlQuery q;
    int count = 0;
    q.exec("SELECT * FROM ClientSatellite");
    while(q.next()) {
        satData sdata;
        count++;
        sdata.setNoradID(q.value(0).toInt());
        sdata.setSatName(q.value(1).toString());
        sdata.setLaunchYear(q.value(2).toInt());
        sdata.setLaunchNumber(q.value(3).toInt());
        sdata.setFullEpoch(q.value(4).toDouble());
        sdata.setEpochYear(q.value(5).toInt());
        sdata.setEpochDay(q.value(6).toInt());
        sdata.setEpochTime(q.value(7).toDouble());
        sdata.setDragCoefficient(q.value(8).toDouble());
        sdata.setOrbitInclination(q.value(9).toDouble());
        sdata.setRightAscentionAscendingNode(q.value(10).toDouble());
        sdata.setEccentricity(q.value(11).toDouble());
        sdata.setPerigee(q.value(12).toDouble());
        sdata.setMeanAnomaly(q.value(13).toDouble());
        sdata.setMeanMotion(q.value(14).toDouble());
        satList.push_back(sdata);
    }
    qDebug() << "size: " << satList.size();
    qDebug() << "count: " << count;

    // test window for table view
    QMessageBox msgBox;
    QProgressBar *gress = new QProgressBar;
    gress->setRange(0,count);
    gress->setValue(satList.size());
    if(!(count==satList.size())) {
        msgBox.information(this, "Database Warning","Table download error. \nPlease try again.",QMessageBox::Retry);
        return;
    }

    QDialog d;
    d.setWindowTitle("Table");
    QVBoxLayout *mainlayout = new QVBoxLayout;
    QPushButton *okButton = new QPushButton("Ok");
    QLabel *label = new QLabel("Table Download Status");
    d.setLayout(mainlayout);

    mainlayout->addWidget(label);
    mainlayout->addSpacing(15);
    mainlayout->addWidget(gress);
    mainlayout->addSpacing(15);
    mainlayout->addWidget(okButton);

    QObject::connect(okButton, SIGNAL(clicked()), &d, SLOT(close()));

    // force modal behavior
    d.exec();

    ui->statusBar->showMessage(">> Select Map location");
    ui->tableButton->setDisabled(true);
    ui->locationButton->setDisabled(false);
    ui->calibrateButton->setDisabled(true);
    ui->animateButton->setDisabled(true);

}



/*********************************************************************
Function gets location from User via mouse clicks on Map display.
**********************************************************************/
void MainWindow::getLocation() {

    QDialog d;

    QSize s = d.size();  // set fixed window size
    d.setMaximumSize(s);

    d.setWindowTitle("Map");
    QVBoxLayout *mainlayout = new QVBoxLayout;
    QPushButton *okButton = new QPushButton("Ok");
    label = new customLabel;
    //    label->setPixmap(QPixmap(":/images/cropped_mercator_no_degrees.png").scaled(label->size()));
    label->setPixmap(QPixmap(":/images/cropped_mercator_no_degrees.png"));
    d.setLayout(mainlayout);

    mainlayout->addWidget(label);
    mainlayout->addSpacing(15);
    mainlayout->addWidget(okButton);

    QObject::connect(okButton, SIGNAL(clicked()), &d, SLOT(close()));
    QObject::connect(label, SIGNAL(updateCursor(QPointF)), this, SLOT(getCursorPos(QPointF)));

    // force modal behavior
    d.exec();

    ui->statusBar->showMessage(">> Select measurements");
    ui->tableButton->setDisabled(true);
    ui->locationButton->setDisabled(true);
    ui->calibrateButton->setDisabled(false);
    ui->animateButton->setDisabled(true);

}



/**********************************************************************
Function asks User to set Boresight value.
***********************************************************************/
void MainWindow::getBoresight() {

    QDialog d;
    d.setWindowTitle("Values");
    QVBoxLayout *mainlayout = new QVBoxLayout;
    QHBoxLayout *horizlayout = new QHBoxLayout;
    QHBoxLayout *horiz2 = new QHBoxLayout;
    QHBoxLayout *horiz3 = new QHBoxLayout;
    QPushButton *okButton = new QPushButton("Ok");
    QDial *dial = new QDial;
    QSlider *slider = new QSlider;
    QLabel *dLabel = new QLabel("0");
    QLabel *sLabel = new QLabel("0");
    QLabel *dRite = new QLabel("Azimuth");
    QLabel *sRite = new QLabel("Elevation");

    QFont f;
    f.setBold(true);
    dRite->setFont(f);
    sRite->setFont(f);

    dLabel->setFrameStyle(1);
    sLabel->setFrameStyle(1);

    dial->setMinimum(0);
    dial->setMaximum(359);
    dial->setWrapping(true);
    dial->setNotchesVisible(true);
    dial->setToolTip("select 0 to 360 degrees");

    slider->setMinimum(0);
    slider->setMaximum(90);
    slider->setPageStep(5);
    slider->setToolTip("select 0 to 90 degrees");

    d.setLayout(mainlayout);
    mainlayout->addLayout(horiz3);
    mainlayout->addSpacing(15);
    mainlayout->addLayout(horiz2);
    mainlayout->addSpacing(15);
    mainlayout->addLayout(horizlayout);

    horiz3->addWidget(dRite);
    horiz3->addSpacing(35);
    horiz3->addWidget(sRite);

    horiz2->addWidget(dLabel);
    horiz2->addSpacing(35);
    horiz2->addWidget(sLabel);

    horizlayout->addWidget(dial);
    horizlayout->addSpacing(35);
    horizlayout->addWidget(slider);
    mainlayout->addWidget(okButton);


    QObject::connect(dial, SIGNAL(valueChanged(int)), dLabel, SLOT(setNum(int)));
    QObject::connect(slider, SIGNAL(valueChanged(int)), sLabel, SLOT(setNum(int)));
    QObject::connect(okButton, SIGNAL(clicked()), &d, SLOT(close()));

    // force modal behavior
    d.exec();

    // pass Azimuth and Elevation values to main gui window variables and labels
    this->ui->userAzimuth->setText(dLabel->text());
    this->userAzimuthValue = dLabel->text().toInt();
    qDebug()<<"Azimuth value = "<<getAzimuthValue();

    this->ui->userElevation->setText(sLabel->text());
    this->userElevationValue = sLabel->text().toInt();
    qDebug()<<"Elevation value = "<<getElevationValue();


    // set buttons for next user input
    ui->statusBar->showMessage(">> Ready to Animate");
    ui->tableButton->setDisabled(true);
    ui->locationButton->setDisabled(true);
    ui->calibrateButton->setDisabled(true);
    ui->animateButton->setDisabled(false);

}



/*************************************************************************************
Function initiates Animation window and text browser window for satellite display.
**************************************************************************************/
void MainWindow::getAnimate() {


    //    QDialog d;

    //    d.setWindowTitle("Animation");
    //    QVBoxLayout *mainlayout = new QVBoxLayout;
    //    QHBoxLayout *horiz1 = new QHBoxLayout;
    //    QHBoxLayout *horiz2 = new QHBoxLayout;
    //    QGraphicsScene *gscene = new QGraphicsScene;
    //    QGraphicsView *gview = new QGraphicsView;
    //    QTextEdit *textEdit = new QTextEdit;
    //    QPushButton *quitButton = new QPushButton("Quit");
    //    QPushButton *pauseButton = new QPushButton("Pause");


    ////    QPixmap pixmap(":/images/test_starry_back.png");
    //    QPixmap pixmap(":/images/test_starry_night_back.png");
    //    gscene->setSceneRect(0,0,pixmap.width(),pixmap.height());
    //    gscene->addPixmap(pixmap);
    //    gview->setScene(gscene);
    //    gview->setSceneRect(0,0,pixmap.width(),pixmap.height());

    //    gview->fitInView(gscene->itemsBoundingRect(),Qt::KeepAspectRatioByExpanding);
    ////    gview->scale(1.25,1.25);  // golden method that works with zoom & resize too!
    //    gview->scale(1.15,1.15);  // must manually scale each background pic to view
    //    gview->setFixedWidth(pixmap.width()/1.5);  // only affects size of window not actual image size!
    //    gview->setFixedHeight(pixmap.height()/1.5);  // only affects size of window not actual image size!

    ////    qDebug()<<"pixmap width "<<pixmap.width();
    ////    qDebug()<<"pixmap height "<<pixmap.height();
    ////    qDebug()<<"scene width "<<gscene->width();
    ////    qDebug()<<"scene height "<<gscene->height();
    ////    qDebug()<<"view width "<<gview->width();
    ////    qDebug()<<"view height "<<gview->height();

    //    gview->setRenderHint(QPainter::Antialiasing);
    //    gview->setMouseTracking(true);

    //    d.setLayout(mainlayout);
    //    textEdit->setFrameStyle(1);
    //    textEdit->setReadOnly(true);
    //    textEdit->setWindowIconText("Satellite Info");
    //    mainlayout->addLayout(horiz1);
    //    mainlayout->addSpacing(20);
    //    mainlayout->addLayout(horiz2);

    //    horiz1->addWidget(gview);
    //    horiz1->addSpacing(15);
    //    horiz1->addWidget(textEdit);

    //    horiz2->addWidget(pauseButton);
    //    horiz2->addSpacing(150);
    //    horiz2->addWidget(quitButton);


    //    QObject::connect(pauseButton, SIGNAL(clicked()), pauseButton, SLOT(toggle()));
    //    QObject::connect(quitButton, SIGNAL(clicked()), &d, SLOT(close()));
    //    QObject::connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    //    // force modal behavior
    //    d.exec();


/***************************************************************************************************
 Start various animation windows HERE!
****************************************************************************************************/

//    ad = new AnimateDialog(this);
//    ad->setAllParameters(&satList, userMapCoordinates.ry(), userMapCoordinates.rx(), getAzimuthValue(), getElevationValue(), 45, 325);
//    ad->show();


    az = new AnimateD2(this);
    az->setAllParameters(&satList, userMapCoordinates.ry(), userMapCoordinates.rx(), getAzimuthValue(), getElevationValue(), 45, 325);
    az->show();


//    ui->tableButton->setDisabled(false);
    ui->locationButton->setDisabled(false);
    ui->calibrateButton->setDisabled(false);
    ui->animateButton->setDisabled(false);

}



/**************************************************************************************************
Function receives mouse cursor position from Map dialog and sets appropriate labels in main window.
***************************************************************************************************/
void MainWindow::getCursorPos(QPointF zet) { // pass cursor position to main window

    qDebug()<<"Map cursor pos = "<<zet;

    // Instantiate the utility to convert pixel coordinates to lat/lon
    LLfromXY util;
    util.setMapValues(420, 760, 50, 24, -126, -66);
    QPointF temp(util.getLongitude(zet.rx()), util.getLatitude(zet.ry()));
    double userLat = util.getLatitude(zet.ry());
    double userLon = util.getLongitude(zet.rx());
    userMapCoordinates = temp;
    ui->userMapValue->setText(QString("Lat %1, Long %2").arg((int)userLat).arg((int)userLon));
}




/***************************************************************************************
Function returns User Map Coordinates.
****************************************************************************************/
QPointF MainWindow::getMapCoordinates() {

    return userMapCoordinates;
}




/*****************************************************************************************
Function returns User Azimuth value.
*****************************************************************************************/
int MainWindow::getAzimuthValue() {

    return userAzimuthValue;

}




/*****************************************************************************************
Function returns User Elevation value.
******************************************************************************************/
int MainWindow::getElevationValue() {

    return userElevationValue;

}


/*****************************************************************************************
Function returns true if database connection established.
******************************************************************************************/
bool MainWindow::connectToDatabase() {
    db = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL"));//QSqlDatabase::addDatabase("QMYSQL");
    db->setHostName("pavelow.eng.uah.edu");
    db->setPort(3301);
    db->setUserName("team1");
    db->setPassword("satview14");
    db->setDatabaseName("ClientTool");
    QString msg = db->lastError().text();
    qDebug() << msg;

    if(!db->open())
    {
        QMessageBox msgBox;
        // Two cases here - bad credentials, or DB unavailable
        QString msg = db->lastError().text();
        qDebug() << msg;
        qDebug() << "Error unable to connect due to above error";
        if (msg.contains("Unknown MySQL server host")) {
            msgBox.information(this, "Database Unavailable.","Please retry later.",QMessageBox::Retry);
            return false;
        } else if (msg.contains("Access denied")) {
            msgBox.information(this, "Incorrect User Info","Either username or password is incorrect.\nPlease re-enter username and password.",QMessageBox::Retry);
            return false;
        } else {
            msgBox.information(this, "Unknown MySQL failure:",msg,QMessageBox::Retry);
            return false;
        }


    } else {
        qDebug() << "connected";
    }

    return true;
}


void MainWindow::testSlot(int cnt) {

    //test slot
    qDebug()<<"count = "<<cnt;
}



MainWindow::~MainWindow()
{
    delete ui;
}
