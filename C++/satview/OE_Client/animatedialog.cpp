#include "animatedialog.h"
#include "ui_animatedialog.h"


AnimateDialog::AnimateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnimateDialog)
{
    ui->setupUi(this);

    connect(ui->timeEdit, SIGNAL(timeChanged(QTime)), this, SLOT(updateSelectedTime(QTime)));
    // Used if user clicks a day
    connect(ui->calendarWidget, SIGNAL(clicked(QDate)), this, SLOT(updateSelectedDate(QDate)));
    // Used if user changes month or year
    connect(ui->calendarWidget, SIGNAL(currentPageChanged(int,int)), this, SLOT(updateMonthOrYear(int,int)));
    connect(ui->timeButton, SIGNAL(clicked()), this, SLOT(useCustomTimeActionPerformed()));

    // Set a black background
//    myScene = new QGraphicsScene(this);
//    QPixmap pixmapz(":/images/test_starry_night_back.png");
//    myScene->setSceneRect(0,0,pixmapz.width(),pixmapz.height());
//    myScene->addPixmap(pixmapz);
//    ui->graphicsView->setScene(myScene);
//    ui->graphicsView->setSceneRect(0,0,pixmapz.width(),pixmapz.height());
//    ui->graphicsView->scale(0.5585,0.7046);  // must manually scale background pic to view
    // Set a black background
    myScene = new QGraphicsScene(this);
    myScene->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));
    myScene->setItemIndexMethod(QGraphicsScene::NoIndex);
    ui->graphicsView->setScene(myScene);

    // Draw the FOV circle
    QPen penCirc(Qt::white);
    QBrush brushCirc(Qt::white, Qt::BrushStyle(Qt::transparent));
    myScene->addEllipse(0, 0,ui->graphicsView->width()-10, ui->graphicsView->height()-10, penCirc, brushCirc);

    // Set up the timer
    myTimer = new QTimer(this);
    myTimer->setInterval(1000);
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(startButtonActionPerformed()));
    connect(myTimer, SIGNAL(timeout()), this, SLOT(updateDisplay()));

    // Drawings
    pen = new QPen(Qt::transparent);
    brush = new QBrush(Qt::white, Qt::BrushStyle(Qt::SolidPattern));
    numSatsPrefix = "Sats in view: ";

    //connect stmts
//    connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(closeApp()));

}

AnimateDialog::~AnimateDialog()
{
    delete ui;
}

void AnimateDialog::startButtonActionPerformed() {
    if (ui->startButton->text().contains("Start")) {
        if (!timeInitialized) {
            // For Acceptance Test 06-02, pop a message box
            QString newTimeForMsgbox("No Time Selected!\n");
            newTimeForMsgbox.append("If time is not selected, current system time will be used.\n");
            newTimeForMsgbox.append("Click OK to use current system time, or CANCEL to choose custom time.");
            QMessageBox msgBox;
            int reply = msgBox.information(this, "Default Time Selected",newTimeForMsgbox,QMessageBox::Ok, QMessageBox::Cancel);
            if (reply == QMessageBox::Cancel) return;
            qDebug() << "Start time: " << QString::number(time(&unixTimeClock));
        }
        // Start the timer
        myTimer->start();
        // toggle button text to Stop
        ui->startButton->setText("Stop Animation");

    } else {
        // Stop the timer
        myTimer->stop();
        // toggle buttontext to Start
        ui->startButton->setText("Start Animation");
    }
}

void AnimateDialog::updateSelectedTime(QTime aTime) {
    qDebug() << aTime.toString();
    userTime = aTime;
}

void AnimateDialog::updateSelectedDate(QDate aDate) {
    qDebug() << aDate.toString();
    userDate = aDate;
}

void AnimateDialog::updateMonthOrYear(int y, int m) {
    qDebug() << "changing y: " << QString::number(y) << ", m: " << QString::number(m);
    QDate date = ui->calendarWidget->selectedDate();
    date.setDate(y, m, 1);
    ui->calendarWidget->setSelectedDate(date);
}

void AnimateDialog::useCustomTimeActionPerformed() {
    if (userTime.isNull()) userTime = ui->timeEdit->time();
    if (userDate.isNull()) userDate = ui->calendarWidget->selectedDate();
    qDebug() << "Y: " << userDate.year() << ", M: " << userDate.month() << ", D: " << userDate.day();
    userTime1970 = (userTime.second() + userTime.minute()*60 +userTime.hour()*3600) + (userDate.daysTo(QDate(1970, 1, 1)) * -24 * 60 * 60);
    timeInitialized = true;
    qDebug() << "Unix time: " << QString::number(userTime1970);
    // Pop a dialog for demo purposes with the new time
    QString newTimeForMsgbox("New Time Selected.\n");
    newTimeForMsgbox.append(userTime.toString());
    newTimeForMsgbox.append("\n");
    newTimeForMsgbox.append(userDate.toString());
    newTimeForMsgbox.append("\n");
    newTimeForMsgbox.append("Unix time: ");
    newTimeForMsgbox.append(QString::number(userTime1970));
    QMessageBox msgBox;
    msgBox.information(this, "Custom Time Selected",newTimeForMsgbox,QMessageBox::Ok);
}

void AnimateDialog::updateDisplay() {

//    myScene->clear();
//    // Draw the FOV circle
//    QPen penCirc(Qt::white);
//    QBrush brushCirc(Qt::white, Qt::BrushStyle(Qt::transparent));
//    myScene->addEllipse(0, 0,ui->graphicsView->width()-10, ui->graphicsView->height()-10, penCirc, brushCirc);

    QGraphicsPixmapItem *pixmapItem;
    QPixmap pixmap(10,10);
    pixmap.fill(Qt::transparent);
    QPainter pixPaint(&pixmap);

    // Use current time unless user pressed the time button
    if (!timeInitialized) {
        satellites = manager->getSatellites(time(&unixTimeClock));
    } else {
        satellites = manager->getSatellites(userTime1970);
        userTime1970++;
    }

    numSatsFull.clear();
    numSatsFull.append(numSatsPrefix);
    numSatsFull.append(QString::number(satellites->size()));
    ui->numSatsLabel->setText(numSatsFull);
    // Iterate through satellites and add them to the scene
    QList<Satellite>::iterator s;
    for (s = satellites->begin(); s != satellites->end(); s++) {
        int X = 0, Y = 0;
        double scale = 0;
        QString info;
        int id = 0;
        (*s).getData(X, Y, scale, info, id);
        // Draw item on screen
        brush->setColor(getColor(id));
        pixPaint.setPen(*pen);
        pixPaint.setBrush(*brush);
        pixPaint.drawEllipse(0,0, 5, 5);
        pixmap.scaled(10*scale, 10*scale, Qt::IgnoreAspectRatio, Qt::FastTransformation);
        pixmapItem = myScene->addPixmap(pixmap);
        pixmapItem->setPos(X, Y);
//        myScene->addEllipse(X,Y,10*scale, 10*scale, (*pen), (*brush));
    }
}

void AnimateDialog::setAllParameters(QList<satData> *aList, double lat, double lon,
                                     double boreAZ, double boreEL, double fovDegrees,
                                     int radiusInPixels) {

    manager = new SatelliteManager((*aList));
    // Elevation hardwired to zero
    manager->setViewpoint(lat, lon, 0, boreAZ, boreEL, fovDegrees, radiusInPixels);
}

QColor AnimateDialog::getColor(int id) {
    // Function is probably wildly inefficient
    // Create an RGB value from the ID
    double maxScaled = 0;
    double minScaled = 16777215;
    double maxID = 40000;
    double minID = 1;
    // Make outliers conform (not yet a problem in 2014, we have < 40K objects in the catalog)
    if (id > maxID) id = maxID;
    if (id < minID) id = minID;
    // RGB integer
    int rgbInt = ((maxScaled - minScaled) / (maxID - minID))*(id-minID) + minScaled;
    return QColor(QRgb(rgbInt));
}

//void AnimateDialog::closeApp() {

//    //Send signal to fully close main window
//    emit sendQuit();
//   // this->close();
//    qDebug()<<"emit sendQuit sig from animateDialog";
//}
