#include "animated2.h"
#include "ui_animated2.h"

AnimateD2::AnimateD2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnimateD2)
{
    ui->setupUi(this);

    loadSats();

    timeInitialized = false;
    userTime1970 = (userTime.second() + userTime.minute()*60 +userTime.hour()*3600) + (userDate.daysTo(QDate(1970, 1, 1)) * -24 * 60 * 60);

    myScene = new CustomGraphicsScene(this);
    bg = new QPixmap(":/images/test_starry_night_backJWJ3.png");
    myScene->addPixmap(*bg);
    myScene->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));
    myScene->setItemIndexMethod(QGraphicsScene::NoIndex);
    ui->graphicsView->setScene(myScene);

    // Draw the FOV circle
    QPen penCirc(Qt::transparent);
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
    group = new QGraphicsItemGroup();
    myScene->addItem(group);
    groupRemoved = false;
    showTrails = false;

    userTime1970 = 0;
    animationTime = 0;
    multiplier = 1;
    maximumRange = 2000;
    minimumRange = 200;
    timeInitialized = false;


    // connect statements
    connect(ui->timeEdit, SIGNAL(timeChanged(QTime)), this, SLOT(updateSelectedTime(QTime)));
    // Used if user clicks a day
    connect(ui->calendarWidget, SIGNAL(clicked(QDate)), this, SLOT(updateSelectedDate(QDate)));
    // Used if user changes month or year
    connect(ui->calendarWidget, SIGNAL(currentPageChanged(int,int)), this, SLOT(updateMonthOrYear(int,int)));
    connect(ui->timeButton, SIGNAL(clicked()), this, SLOT(useCustomTimeActionPerformed()));
    connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(stopTime()));  // must stop timer for main window to function properly after animate dialog closes
    // Clear button
    connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(clearButtonActionPerformed()));
    // Max range slider
    connect(ui->maxRngSlider, SIGNAL(valueChanged(int)), this, SLOT(maxRngSliderActionPerformed(int)));
    // Min range slider
    connect(ui->minRngSlider, SIGNAL(valueChanged(int)), this, SLOT(minRngSliderActionPerformed(int)));
    // Time multiplier spin box
    connect(ui->multiplierSpinBox, SIGNAL(valueChanged(int)), this, SLOT(timeMultiplierActionPerformed(int)));
    connect(ui->trailsCheckBox, SIGNAL(clicked()), this, SLOT(showTrailsCheckboxActionPerformed()));
    // Satellite info
    connect(myScene, SIGNAL(updateString(QString)), this, SLOT(getSatString(QString)));

}

AnimateD2::~AnimateD2() {
    delete ui;
    delete satPics;
    delete cus;
    delete bg;
    delete group;
}

void AnimateD2::showTrailsCheckboxActionPerformed() {
    if (ui->trailsCheckBox->isChecked()) {
        showTrails = true;
    } else {
        showTrails = false;
    }
}

void AnimateD2::timeMultiplierActionPerformed(int multi) {
    multiplier = multi;
}

void AnimateD2::maxRngSliderActionPerformed(int maxRng) {
    checkRangeBounds(false, maxRng);
    manager->setMaxRange(maximumRange);
    ui->maxRngLabel->setText(QString::number(maximumRange));
    ui->maxRngSlider->setValue(maximumRange);
}

void AnimateD2::minRngSliderActionPerformed(int minRng) {
    checkRangeBounds(true, minRng);
    manager->setMinRange(minimumRange);
    ui->minRngLabel->setText(QString::number(minimumRange));
    ui->minRngSlider->setValue(minimumRange);
}

void AnimateD2::checkRangeBounds(bool min, int value) {
    if (min) {
        // Minimum range slider changed
        // Min must be at least 100 KM less than max
        if (value > (maximumRange-100)) minimumRange = maximumRange-100;
        else minimumRange = value;
    } else {
        // Maximum range slider changed
        // Max must be at least 100 KM greater than min
        if (value < (minimumRange+100)) maximumRange = minimumRange+100;
        else maximumRange = value;
    }
}

void AnimateD2::updateTimestring(qint64 time) {
    time_t temp(time);
    struct tm *timeinfo;
    if (timeInitialized) {
        timeinfo = gmtime(&temp);
    } else {
        timeinfo = localtime(&temp);
    }
    QString timestring(asctime(timeinfo));
    ui->timeLabel->setText(timestring);
}

void AnimateD2::clearButtonActionPerformed() {
    myScene->removeItem(group);
    group->childItems();
    QList<QGraphicsItem*> items = group->childItems();
    for (int i = 0; i < items.length(); i++) {
        group->removeFromGroup(items.at(i));
    }
    myScene->clear();
    myScene->addPixmap(*bg);
    groupRemoved = true;
}


void AnimateD2::startButtonActionPerformed() {
    if (ui->startButton->text().contains("Start")) {
        if (!timeInitialized) {
            // For Acceptance Test 06-02, pop a message box
            QString newTimeForMsgbox("No Time Selected!\n");
            newTimeForMsgbox.append("If time is not selected, current system time will be used.\n");
            newTimeForMsgbox.append("Click OK to use current system time, or CANCEL to choose custom time.");
            QMessageBox msgBox;
            int reply = msgBox.information(this, "Default Time Selected",newTimeForMsgbox,QMessageBox::Ok | QMessageBox::Default, QMessageBox::Cancel);
            if (reply == QMessageBox::Cancel) return;
            qDebug() << "Start time: " << QString::number(time(&unixTimeClock));
            animationTime = time(&unixTimeClock);
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

void AnimateD2::updateSelectedTime(QTime aTime) {
    qDebug() << aTime.toString();
    userTime = aTime;
}

void AnimateD2::updateSelectedDate(QDate aDate) {
    qDebug() << aDate.toString();
    userDate = aDate;
}

void AnimateD2::updateMonthOrYear(int y, int m) {
    qDebug() << "changing y: " << QString::number(y) << ", m: " << QString::number(m);
    QDate date = ui->calendarWidget->selectedDate();
    date.setDate(y, m, 1);
    ui->calendarWidget->setSelectedDate(date);
}

void AnimateD2::useCustomTimeActionPerformed() {
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
    int reply = msgBox.information(this, "Custom Time Selected",newTimeForMsgbox,QMessageBox::Ok);
    if (reply == QMessageBox::Ok) animationTime = userTime1970;
}

void AnimateD2::updateDisplay() {

    if (!groupRemoved) {
        myScene->removeItem(group);
    } else {
        groupRemoved = false;
    }

    myScene->clear();
    myScene->addPixmap(*bg);
    myScene->addItem(group);



    updateTimestring(animationTime);
    satellites = manager->getSatellites(animationTime);

    // Modify the number of sats in view label
    numSatsFull.clear();
    numSatsFull.append(numSatsPrefix);
    numSatsFull.append(QString::number(satellites->size()));
    ui->numSatsLabel->setText(numSatsFull);


//    // Draw the FOV circle
//    QPen penCirc(Qt::transparent);
//    QBrush brushCirc(Qt::transparent, Qt::BrushStyle(Qt::transparent));
//    myScene->addEllipse(0, 0,ui->graphicsView->width()-10, ui->graphicsView->height()-10, penCirc, brushCirc);

    QGraphicsProxyWidget *proxy;

    //IGNORE these statements
//    QPixmap pixmap(10,10);
//    pixmap.fill(Qt::transparent);
//    QPainter pixPaint(&pixmap);


//    // Use current time unless user pressed the time button
//    if (!timeInitialized) {
//        satellites = manager->getSatellites(time(&unixTimeClock));
//    } else {
//        satellites = manager->getSatellites(userTime1970);
//        userTime1970++;
//    }

//    numSatsFull.clear();
//    numSatsFull.append(numSatsPrefix);
//    numSatsFull.append(QString::number(satellites->size()));
//    ui->numSatsLabel->setText(numSatsFull);

    QGraphicsPixmapItem *pixmapItem;
    QPixmap pixmap(10,10);
    pixmap.fill(Qt::transparent);
    QPainter pixPaint(&pixmap);

    // Iterate through satellites and add them to the scene
    QList<Satellite>::iterator s;
    for (s = satellites->begin(); s != satellites->end(); s++) {
        int X = 0, Y = 0;
        double scale = 0;
        QString info;
        int id = 0;
        (*s).getData(X, Y, scale, info, id);
        updateSatsViewed(id);

        // grab satellite; grab random pixmap from pixmap list
        // pass random pixmap from list plus satellite object in to custom pix label object
        // add custom pix label to scene
        // use constant value from satellite as prime read for pixmap selection

        if (satPics->size()<=0) {
            qWarning("Satellite image directory empty.");
            return;
        }

        int t = ((id + satPics->size()) % satPics->size());  // calculate unique number from satellite norad id for pixmap image choice
        QPixmap *tp = new QPixmap;
        tp = satPics->operator[](t);  // acquire random image from pixmap container
        cus = new CustomLabelPix(*tp, *s, scale);
        proxy = myScene->addWidget(cus);
        QSize size = cus->size();
        int x_a = X - size.width()/2;
        int y_a = Y - size.height()/2;
        proxy->setPos(x_a,y_a);
        connect(cus, SIGNAL(updateString(QString)), this, SLOT(getSatString(QString)));  // use this inside funct

        if (showTrails) {
            brush->setColor(getColor(id));
            pixPaint.setPen(*pen);
            pixPaint.setBrush(*brush);
            pixPaint.drawEllipse(0,0, 5, 5);
            pixmap.scaled(10*scale, 10*scale, Qt::IgnoreAspectRatio, Qt::FastTransformation);
            pixmapItem = myScene->addPixmap(pixmap);
            pixmapItem->setPos(X, Y);
            group->addToGroup(pixmapItem);
        }
    }
    // Add two more labels
    QString unique("Unique sats viewed: ");
    unique.append(QString::number(satsViewed.size()));
    ui->uniqueSatsLabel->setText(unique);

    // Update the time
    animationTime += multiplier;
}

void AnimateD2::setAllParameters(QList<satData> *aList, double lat, double lon,
                                     double boreAZ, double boreEL, double fovDegrees,
                                     int radiusInPixels) {

    manager = new SatelliteManager((*aList));
    // Elevation hardwired to zero
    manager->setViewpoint(lat, lon, 0, boreAZ, boreEL, fovDegrees, radiusInPixels);
}

QColor AnimateD2::getColor(int id) {
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

void AnimateD2::loadSats() {  // run this function in animated2 constructor

    satPics = new QList<QPixmap*>;

    QDir dir(":/images/satpics/");  // initiate directory object with path to directory containing .png files
    if (!dir.exists()) {
        qWarning("Satellite image directory not found.");
        return;
    }

    QFileInfoList sList = dir.entryInfoList();  // create file info list object containing paths to .png files
    qDebug()<<"sList size = "<<sList.size();  // see list size (number of paths to .png files)



    for(int i=0; i<sList.size(); i++) {  // build pixmap list of satellite images
        QFileInfo fileInfo = sList.at(i);
        QPixmap *pix = new QPixmap(fileInfo.filePath());
        qDebug()<<fileInfo.filePath();
        satPics->push_back(pix);
    }

    if(satPics->isEmpty()) {qDebug()<<"satPic list empty";}
    else {qDebug()<<"satPic size = "<<satPics->size();}
}


void AnimateD2::getSatString(QString str) {
    ui->plainTextEdit->appendPlainText(str);
}

void AnimateD2::stopTime() {
    // stop timer; stop all animations
    myTimer->stop();
}

void AnimateD2::updateSatsViewed(int ID) {
    bool viewed = false;
    QList<int>::iterator v;
    for (v = satsViewed.begin(); v != satsViewed.end(); v++) {
        if (ID == (*v)) {
            viewed = true;
            break;
        }
    }
    if (!viewed) {
        satsViewed.push_back(ID);
    }
}



