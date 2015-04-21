#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    ui->submitEdit->installEventFilter(this);

    setWindowTitle("Chatter Monkey");

    //  initialize sockets
    aprsSocket = new QTcpSocket(this);

    chatSocket = new QUdpSocket(this);
    recevSocket = new QUdpSocket(this);

    serverFlag = false;
    chatFlag = true;

    portNumber = APRSPORT;

    dumInt = (qrand() + 9999 ) % 9999;

    // intialize tabs
    ui->tabWidget->widget(0)->setFocus();
    ui->tabWidget->setCurrentWidget(0);
    ui->tabWidget->setTabText(0, "APRS");
    ui->tabWidget->setTabText(1, "Chat");


    // set fixed window size
    QSize s = this->size();
    this->statusBar()->setSizeGripEnabled(false);
    this->setFixedSize(s);


    // center main window on screen at execution
    QRect position = this->frameGeometry();
    position.moveCenter(QDesktopWidget().availableGeometry().center());
    move(position.topLeft());


    // add logos
//    ui->logoLabel->setPixmap(QPixmap(":/images/monkey03_flip.jpg").scaled(ui->logoLabel->size(),Qt::KeepAspectRatioByExpanding));
    ui->logoLabel->setPixmap(QPixmap(":/images/monkey03_flip02.png").scaled(ui->logoLabel->size(),Qt::KeepAspectRatioByExpanding));


    // set status label & actions
    statusLabel = new QLabel("Ready");
//    this->statusBar()->addWidget(statusLabel);
    ui->statusBar->addWidget(statusLabel);


    // set action menu shortcuts
    quitAction = new QAction(tr("Quit"), this);
    quitAction->setShortcut(QKeySequence::Quit);
    saveAction = new QAction(tr("Save All"), this);
    saveAction->setShortcut(QKeySequence::Save);
    helpAction = new QAction(tr("Help"), this);
    helpAction->setShortcut(QKeySequence(Qt::ControlModifier + Qt::Key_H));
    aboutAction = new QAction(tr("About"), this);
    aboutAction->setShortcut(QKeySequence(Qt::ControlModifier + Qt::Key_A));
    luckyAction = new QAction(tr("Lucky"),this);
    luckyAction->setShortcut(QKeySequence(Qt::ControlModifier + Qt::Key_L));

    fileMenu = new QMenu;
    fileMenu = ui->menuBar->addMenu("&File");
    fileMenu->addAction(saveAction);
    fileMenu->addAction(quitAction);
    luckyMenu = new QMenu;
    luckyMenu = ui->menuBar->addMenu("&Special");
    luckyMenu->addAction(luckyAction);
    helpMenu = new QMenu;
    helpMenu = ui->menuBar->addMenu("&Help");
    helpMenu->addAction(helpAction);
    helpMenu->addAction(aboutAction);


    // include output readme file [in constructor] explaining save all function; monkey_save.txt


    // *** APRS module ***
//    ui->aprsEdit->setStyleSheet("color: blue;"
//                            "background-color: yellow;"
//                            "selection-color: yellow;"
//                            "selection-background-color: blue;");

    ui->aprsEdit->setStyleSheet("color: white;"
                            "background-color: black;"
                            "selection-color: black;"
                            "selection-background-color: white;"
                            "border: 2px solid blue");

    //input validators
//    QRegularExpressionValidator* validate1 = new QRegularExpressionValidator(QRegularExpression("[A-Za-z0-9]{0,7}"),ui->callSignEdit);
//    ui->callSignEdit->setValidator(validate1);


//    QRegularExpressionValidator* validate2 = new QRegularExpressionValidator(QRegularExpression("[A-Za-z0-9]{0,22}"),ui->nameEdit);
//    QRegExp re("[_a-zA-Z][_a-zA-Z0-9]+");
    QRegExp re("[_a-zA-Z][_a-zA-Z0-9]{0,22}");
    QRegExpValidator *vi = new QRegExpValidator(re);
    ui->nameEdit->setValidator(vi);

    QDoubleValidator* intValidator1 = new QDoubleValidator(15, 60, 5, ui->latitudeEdit);   // mid Central America to mid Canada
    ui->latitudeEdit->setValidator(intValidator1);

    QDoubleValidator* intValidator2 = new QDoubleValidator(-135, -45, 5, ui->longitudeEdit);   // mid Pacific to mid Alantic
    ui->longitudeEdit->setValidator(intValidator2);

    QIntValidator* intVal3 = new QIntValidator(0, 65535, ui->portNumEdit);
    ui->portNumEdit->setValidator(intVal3);

    QIntValidator* intVal4 = new QIntValidator(0, 65535, ui->recevPortNumEdit);
    ui->recevPortNumEdit->setValidator(intVal4);

    QRegExpValidator *v = new QRegExpValidator(this);
    QRegExp rx("((1{0,1}[0-9]{0,2}|2[0-4]{1,1}[0-9]{1,1}|25[0-5]{1,1})\\.){3,3}(1{0,1}[0-9]{0,2}|2[0-4]{1,1}[0-9]{1,1}|25[0-5]{1,1})");
    v->setRegExp(rx);
    ui->ipAddrEdit->setValidator(v);

    ui->connectButton->setText("Connect");
    ui->connectButton->setFocus();
    ui->serverAprsButton->setChecked(true);
    ui->hsvAlButton->setChecked(true);

    // to agree with hsvAlButton set to true
    ui->mapButton->setDisabled(true);
    ui->latitudeEdit->setDisabled(true);
    ui->longitudeEdit->setDisabled(true);
    ui->latitudeEdit->setText("34.73");
    ui->longitudeEdit->setText("-86.58");

//    ui->latitudeEdit->clear();
//    ui->longitudeEdit->clear();

//    ui->callSignEdit->clear();
    ui->callSignEdit->setPlaceholderText("Callsign");
    ui->sentBytes->clear();
    ui->recevBytes->clear();

    ui->reportEdit->clear();
    ui->timeEdit->clear();
    ui->tempEdit->clear();
    ui->gustEdit->clear();
    ui->windSpdEdit->clear();
    ui->windDirEdit->clear();
    ui->baroEdit->clear();
    ui->humidEdit->clear();
    ui->rainHourEdit->clear();
    ui->rainMidEdit->clear();
    ui->aprsEdit->clear();

//    ui->latitudeEdit->setReadOnly(true);  // allow user to input directly as well as pick map coordinates
//    ui->longitudeEdit->setReadOnly(true);

    ui->reportEdit->setReadOnly(true);
    ui->timeEdit->setReadOnly(true);
    ui->tempEdit->setReadOnly(true);
    ui->gustEdit->setReadOnly(true);
    ui->windSpdEdit->setReadOnly(true);
    ui->windDirEdit->setReadOnly(true);
    ui->baroEdit->setReadOnly(true);
    ui->humidEdit->setReadOnly(true);
    ui->rainHourEdit->setReadOnly(true);
    ui->rainMidEdit->setReadOnly(true);
    ui->aprsEdit->setReadOnly(true);


    // chat initialize
    ui->groupBox_4->setDisabled(true);
    ui->converseEdit->setReadOnly(true);
    ui->converseEdit->setText("*** Ready to Receive ***\n\n");
    ui->nameEdit->setPlaceholderText("Name");
    ui->ipAddrEdit->setPlaceholderText("IP Address (ipv4)");
//    ui->ipAddrEdit->setText("10.0.0.1");  // testing
    ui->portNumEdit->setPlaceholderText("Send Port");
    ui->recevPortNumEdit->setPlaceholderText("Recv Port");
    ui->warningLabel->setText("Ready to connect.");



    // connect stmts
    connect(quitAction, SIGNAL(triggered()), this, SLOT(quitMessage()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveMessage()));
    connect(helpAction, SIGNAL(triggered()), this, SLOT(helpMessage()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(aboutMessage()));
    connect(luckyAction, SIGNAL(triggered()), this, SLOT(luckyMessage()));

    connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(writeMessage()));
    connect(aprsSocket, SIGNAL(readyRead()), this, SLOT(processMessage()));
    connect(ui->clearPacketsButton, SIGNAL(clicked()), this, SLOT(clearPackets()));
    connect(ui->otherButton, SIGNAL(clicked()), this, SLOT(otherMapSelect()));
    connect(ui->hsvAlButton, SIGNAL(clicked()), this, SLOT(hsvMapSelect()));
    connect(ui->serverAprsButton, SIGNAL(clicked()), this, SLOT(setServerFlag()));
    connect(ui->serverUahButton, SIGNAL(clicked()), this, SLOT(setServerFlag()));
    connect(ui->mapButton, SIGNAL(clicked()), this, SLOT(getLocation()));

    connect(ui->chatConnectButton, SIGNAL(clicked()), this, SLOT(startChat()));
    connect(ui->chatDisconnectButton, SIGNAL(clicked()), this, SLOT(chatDisconnect()));
    connect(ui->enterButton, SIGNAL(clicked()), this, SLOT(zinMessage()));
    connect(recevSocket, SIGNAL(readyRead()), this, SLOT(processPendingDataGrams()));
    connect(ui->clearChatButton, SIGNAL(clicked()), this, SLOT(clearChatWindow()));


} // end constructor


void MainWindow::quitMessage() {

    MainWindow dialog;
    QMessageBox msgBox;

    if(QMessageBox::Ok == msgBox.information(&dialog, "Quit","Do you want to quit?",QMessageBox::Ok | QMessageBox::Cancel))
    {
        close();
//        qDebug()<< "Application closed.";
    }
}


void MainWindow::saveMessage() {

    QMessageBox msgBox;

    // Check if old output file exists
    QFile test1("./monkey_save.txt");
    if (test1.exists() && test1.size() == 0) {
        // Bad file, delete it
        qDebug() << "Deleted empty monkey_save.txt file.";
        test1.remove();
    }

    else if (test1.exists() && test1.size() > 0) {
        test1.rename("./monkey_save.txt", "./monkey_save_backup.txt");
    }

    // Prompt user for save file name
    QString str = QFileDialog::getSaveFileName(this, tr("Save file for output"), "./monkey_save.txt", tr("Text files (*.txt)"));

    // Exit function if user cancels
    if (str == NULL) return;

    // Test if file is openable and writable
    QFile test(str);
    test.open(QIODevice::ReadWrite);
    if (!test.isOpen() || !test.isWritable()) {
        QMessageBox::Retry == msgBox.information(this, "Invalid Target!","Choose another file or location.\n",QMessageBox::Retry);
        test.close();
        return;
    }

    // Save output data to file
    QTextStream out(&test);
    out << ui->aprsEdit->document()->toPlainText();
    out << ui->converseEdit->document()->toPlainText();
//    qDebug()<<"File saved.";
    test.close();
}



void MainWindow::helpMessage(){

    MainWindow dialog1;
    QMessageBox help_msgBox;

    help_msgBox.information(&dialog1,"Chatter Monkey Help","Welcome to Chatter Monkey.\n\n"
                            "1.  Get APRS data and weather information.\n"
                            "2.  Chat with clients.\n\n"
                            "Click tab name to use modules.\n"
                            "Save chat logs and APRS output to a text file.\n\n\n"
                            "Troubleshooting Help:\n\n"
                            "Q:  Nothing displays, I see no output anywhere.\n"
                            "A:  Chatter Monkey needs reliable network connection for best results.\n\n"
                            "Q:  You promised a port scanner, where is it?\n"
                            "A:  Chatter Monkey is in beta, port scanning module in development.\n\n"
                            "Q:  I see no text display in chat menus, the internet is working fine.\n"
                            "A:  Enter valid IP address of the DESTINATION client.\n\n"
                            "Q:  Chat still not connecting.  Dest. IP address is correct, pings work.\n"
                            "A:  Chatter Monkey uses peer-to-peer UDP sockets for connecting.\n"
                            "       Your send port should match destination client receive port.\n"
                            "       Your receive port should match destination client send port.\n\n"
                            "Q:  Chat tab is disabled, what happened!?\n"
                            "A:  Once you disconnect from chat, you must restart Chatter Monkey.\n\n"
                            "Q:  Chat is spotty, APRS output data is sketchy.\n"
                            "A:  Network may be dropping packets due to security or signal strength.\n\n"
                            "Please, don't use Chatter Monkey for malicious activities.",
                            QMessageBox::Close|QMessageBox::NoButton);
}



void MainWindow::aboutMessage(){

    MainWindow dialog2;
    QMessageBox about_msgBox;

    about_msgBox.information(&dialog2,"About Chatter Monkey","Chatter Monkey is a freeware program.\n"
                             "Version 1.10\n\n"
                             "Software joint venture of Ninja Boy, zZaZz, & King_Stallion.\n"
                             "Linux Mint and Windows 7+ support.\n"
                             "Coded in C++, compiled with gcc, built in Qt.\n"
                             "2014 May 25 | 2014 June 16 ed.", QMessageBox::Close|QMessageBox::NoButton);

}


void MainWindow::luckyMessage(){

//    qDebug()<<"Lucky slot called.";

    gam = new Gamble(this);
//    gam->exec();
//    gam->focusWidget();
//    gam->setFocus();
//    gam->raise();
//    gam->activateWindow();
    gam->show();

}


void MainWindow::clearPackets() {

    ui->aprsEdit->clear();
    ui->recevBytes->clear();
}



void MainWindow::lockAPRSbuttons() {

    ui->groupBox->setDisabled(true);
    ui->callSignEdit->setDisabled(true);
    ui->clearPacketsButton->setDisabled(true);
    ui->groupBox_2->setDisabled(true);

    if (ui->otherButton->isChecked()) {
        ui->mapButton->setDisabled(true);
        ui->latitudeEdit->setDisabled(true);
        ui->longitudeEdit->setDisabled(true);
    }
}



void MainWindow::unlockAPRSbuttons() {

    ui->groupBox->setDisabled(false);
    ui->callSignEdit->setDisabled(false);
    ui->clearPacketsButton->setDisabled(false);
    ui->groupBox_2->setDisabled(false);

    if (ui->otherButton->isChecked()) {
        ui->mapButton->setDisabled(false);
        ui->latitudeEdit->setDisabled(false);
        ui->longitudeEdit->setDisabled(false);
    }
}



void MainWindow::writeMessage() {

    // pull current values from: callsign edit, server buttons, lat, long edits
    QByteArray d;
    QString serverAddr, cs, outMsg, latVal, longVal;
    int portNum;

    if(ui->connectButton->text()=="Connect") {

        this->clearLineEdits();
        this->lockAPRSbuttons();

        serverAddr = this->getServerName();
        portNum = this->getPortNumber();
        cs = this->getCallSign();
        latVal = this->getLatPos();
        longVal = this->getLongPos();

        aprsSocket->connectToHost(serverAddr, portNum, QIODevice::ReadWrite);

        // build string
        outMsg = "user "+cs+" pass -1 vers testsoftware 1.0_05 filter r/";
        outMsg = outMsg+latVal+"/"+longVal+"/"+RADIUS;

        outMsg=outMsg+"\n";
        d=outMsg.toStdString().c_str();  // QString >> Standard String >> Char Array >> QByteArray
        aprsSocket->write(d);

        int s = d.size();
        ui->sentBytes->setText(QString::number(ui->sentBytes->text().toInt()+s));

//        qDebug()<<outMsg;
//        qDebug()<<"Server Name = "<<serverAddr;
//        qDebug()<<"Port Number = "<<portNum;
//        qDebug()<<"Callsign = "<<cs;
//        qDebug()<<"Lat Value = "<<latVal;
//        qDebug()<<"Long Value = "<<longVal;


        ui->aprsEdit->append("#Connection Established");
        ui->connectButton->setText("Disconnect");
    }

//    else if(ui->connectButton->text()=="Disconnect") {
    else {

        this->unlockAPRSbuttons();
//        this->ui->latitudeEdit->clear();
//        this->ui->longitudeEdit->clear();

        ui->aprsEdit->append("#Disconnected");
        ui->connectButton->setText("Connect");
        aprsSocket->disconnectFromHost();
    }

}


void MainWindow::processMessage() {

    QString rdy(aprsSocket->readLine());
    ui->aprsEdit->append(rdy);

    int t = rdy.size();  // keep track of received bytes
    ui->recevBytes->setText(QString::number(ui->recevBytes->text().toInt()+t));

    //pass rdy string to parse functions
    if (!serverFlag) {this->parseAPRS(rdy);}
    else {this->parseUAH(rdy);}
}


QString MainWindow::getCallSign() {

    // check if empty
    if (ui->callSignEdit->text().isEmpty()) {
        ui->callSignEdit->setText("K9S3X");
        return ("K9S3X");
    }

    else {
        return ui->callSignEdit->text();
    }
}

QString MainWindow::getServerName() {

    if(!serverFlag) {return ("rotate.aprs.net");}
    else {return ("blackhawk.eng.uah.edu");}
}


void MainWindow::setServerFlag() {

    if (ui->serverAprsButton->isChecked()) {
        serverFlag = false;
        this->setPortNumber(APRSPORT);
    }

    else if (ui->serverUahButton->isChecked()) {
        serverFlag = true;
        this->setPortNumber(UAHPORT);  // USA only feeds from UAH server
    }
}


void MainWindow::setPortNumber(int port) {portNumber = port;}

int MainWindow::getPortNumber() {return portNumber;}


QString MainWindow::getLatPos() {

    double dum = ui->latitudeEdit->text().toDouble();

    // check if empty
    if (ui->latitudeEdit->text().isEmpty()) {
        ui->latitudeEdit->setText("34.73");
        return ("34.73");
    }

    // check if value outside bounds
    else if (dum<15 || dum>60) {
        dum = 15 + (qrand() % 45);
        ui->latitudeEdit->setText(QString::number(dum));
        return (QString::number(dum));
    }

    else {return ui->latitudeEdit->text();}
}


QString MainWindow::getLongPos() {

    double dum = ui->longitudeEdit->text().toDouble();

    // check if empty
    if (ui->longitudeEdit->text().isEmpty()) {
        ui->longitudeEdit->setText("-86.58");
        return ("-86.58");
    }

    // check if value outside bounds
    else if (dum<-135 || dum>-45) {
        dum = -135 + (qrand() % -90);
        ui->longitudeEdit->setText(QString::number(dum));
        return (QString::number(dum));
    }

    else {return ui->longitudeEdit->text();}
}


void MainWindow::otherMapSelect() {

    ui->mapButton->setDisabled(false);
    ui->latitudeEdit->setDisabled(false);
    ui->longitudeEdit->setDisabled(false);

    ui->latitudeEdit->clear();
    ui->longitudeEdit->clear();
}


void MainWindow::hsvMapSelect() {

    ui->mapButton->setDisabled(true);
    ui->latitudeEdit->setDisabled(true);
    ui->longitudeEdit->setDisabled(true);

    ui->latitudeEdit->setText("34.73");
    ui->longitudeEdit->setText("-86.58");
}


void MainWindow::getCursorPos(QPointF zet) {

    // Instantiate the utility to convert pixel coordinates to lat/lon
    LLfromXY util;
    util.setMapValues(420, 760, 50, 24, -126, -66);
    double userLat = util.getLatitude(zet.ry());
    double userLon = util.getLongitude(zet.rx());

    ui->latitudeEdit->setText(QString::number(userLat));
    ui->longitudeEdit->setText(QString::number(userLon));
//    qDebug()<<"Map cursor pos = "<<userLat<<""<<userLon;
}


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

}


void MainWindow::parseAPRS(QString pstr) {

    // search criteria in this function work for rotate.aprs.net:10152
    // parsing for APRS Net (rotate.aprs.net) only

    // UAH Blackhawk parse expression contains ":@"
    // see APRS document for parsing details

    // Cap total character length on textedit >> use plaintextedit
    // Cap total bytes received instead, clear app and disconnect

    int i;
    QString dum;

    if(pstr.contains(":_")) {
        i=pstr.indexOf(":_");
        ui->reportEdit->setText(pstr.mid(0,8));

        i=i+2;
        dum = pstr.mid(i,8);
        dum = "Date "+dum.mid(0,2)+"/"+dum.mid(2,2)+"  Time "+dum.mid(4,2)+":"+dum.mid(6,2)+" (24hr)";
        ui->timeEdit->setText(dum);

        if(pstr.contains("c")) {
            i=pstr.indexOf("c");
            dum = pstr.mid(i+1,3);
            ui->windDirEdit->setText(dum+" degrees");
        }

        if(pstr.contains("s")) {
            i=pstr.indexOf("s");
            dum = pstr.mid(i+1,3);
            ui->windSpdEdit->setText(dum+" mph");
        }

        if(pstr.contains("g")) {
            i=pstr.indexOf("g");
            dum = pstr.mid(i+1,3);
            ui->gustEdit->setText(dum+" mph");
        }

        if(pstr.contains("t")) {
            i=pstr.indexOf("t");
            dum = pstr.mid(i+1,3);
            ui->tempEdit->setText(dum+" F");
        }

        if(pstr.contains("b")) {
            i=pstr.indexOf("b");
            dum = pstr.mid(i+1,5);
            ui->baroEdit->setText(dum+" millibars / hPascal");
        }

        if(pstr.contains("h")) {
            i=pstr.indexOf("h");
            dum = pstr.mid(i+1,2);
            ui->humidEdit->setText(dum+" %");
        }

        if(pstr.contains("r")) {
            i=pstr.indexOf("r");
            dum = pstr.mid(i+1,3);
            ui->rainHourEdit->setText(dum+" 1/100 inch");
        }

        if(pstr.contains("p")) {
            i=pstr.indexOf("p");
            dum = pstr.mid(i+1,3);
            ui->rainMidEdit->setText(dum+" 1/100 inch");
        }

    }

}



void MainWindow::parseUAH(QString pstr) {

    int i;
    QString dum, yum;

    if(pstr.contains(":@")) {
        i=pstr.indexOf(":@");
        ui->reportEdit->setText(pstr.mid(0,8));

        i=pstr.indexOf("z");
        i=i+6;
        dum = pstr.mid(i,3);
        i=i+10;
        yum = pstr.mid(i,3);
        ui->timeEdit->setText(dum+" | "+yum);

        if(pstr.contains("c")) {
            i=pstr.indexOf("c");
            dum = pstr.mid(i+1,3);
            ui->windDirEdit->setText(dum+" degrees");
        }

//        if(pstr.contains("s")) {
//            i=pstr.indexOf("s");
//            dum = pstr.mid(i+1,3);
//            ui->windSpdEdit->setText(dum+" mph");
//        }

        if(pstr.contains("g")) {
            i=pstr.indexOf("g");
            dum = pstr.mid(i+1,3);
            ui->gustEdit->setText(dum+" mph");
        }

        if(pstr.contains("t")) {
            i=pstr.indexOf("t");
            dum = pstr.mid(i+1,3);
            ui->tempEdit->setText(dum+" F");
        }

        if(pstr.contains("b")) {
            i=pstr.indexOf("b");
            dum = pstr.mid(i+1,5);
            ui->baroEdit->setText(dum+" millibars / hPascal");
        }

        if(pstr.contains("h")) {
            i=pstr.indexOf("h");
            dum = pstr.mid(i+1,2);
            ui->humidEdit->setText(dum+" %");
        }

        if(pstr.contains("r")) {
            i=pstr.indexOf("r");
            dum = pstr.mid(i+1,3);
            ui->rainHourEdit->setText(dum+" 1/100 inch");
        }

        if(pstr.contains("p")) {
            i=pstr.indexOf("p");
            dum = pstr.mid(i+1,3);
            ui->rainMidEdit->setText(dum+" 1/100 inch");
        }

    }

}


void MainWindow::clearLineEdits() {

    ui->reportEdit->clear();
    ui->timeEdit->clear();
    ui->tempEdit->clear();
    ui->gustEdit->clear();
    ui->windSpdEdit->clear();
    ui->windDirEdit->clear();
    ui->baroEdit->clear();
    ui->humidEdit->clear();
    ui->rainHourEdit->clear();
    ui->rainMidEdit->clear();
    ui->aprsEdit->clear();
}


//bool MainWindow::eventFilter(QObject *watched, QEvent *e) {

//    if (watched==ui->submitEdit && e->type()==QEvent::KeyPress) {
////        QKeyEvent *ke = static_cast<QKeyEvent*>(e);
//        QKeyEvent *ke = (QKeyEvent*)e;

//        if (ke->key()==Qt::Key_Enter || ke->key()==Qt::Key_Return) {
////            sendChat();
//            zinMessage();
//            return true;
//        }

//    }
//    return MainWindow::eventFilter(watched, e);
//}


void MainWindow::startChat() {  // initialize socket; check ip addr, send port, recv port

    QString recv;

    recv = ui->recevPortNumEdit->text();
    ui->warningLabel->clear();
    this->testEmpty();

    if (chatFlag == false) { // check ip, send & recev ports - if empty, do not proceed

        ui->warningLabel->setText("No connection established.\n IP or port fields empty.");
        ui->groupBox_3->setDisabled(false);
        ui->groupBox_4->setDisabled(true);

    }

    else {
        if (this->recevSocket->isOpen() == false) {  // if receive socket not already created, proceed
            this->recevSocket->bind(recv.toInt());
//            this->recevSocket->bind(ui->recevPortNumEdit->text().toInt());
            ui->warningLabel->setText("Connection established.");
            ui->groupBox_3->setDisabled(true);
            ui->groupBox_4->setDisabled(false);
        }
        else {
            ui->warningLabel->setText("Connection already established.\n Restart application to make new connection.");
            ui->groupBox_3->setDisabled(false);
            ui->groupBox_4->setDisabled(true);
        }

    }
}


QString MainWindow::getChatName() {

    QString dum("Bozo Bimbo");
    QString dum2(QString::number(dumInt));

    if (ui->nameEdit->text().isEmpty()) {
        dum = dum+" "+dum2;
        return (dum);
    }
    else {return ui->nameEdit->text();}
}


void MainWindow::testEmpty() {

    if (ui->ipAddrEdit->text().isEmpty()) {chatFlag = false;}
        else {chatFlag = true;}
    if (ui->portNumEdit->text().isEmpty()) {chatFlag = false;}
        else {chatFlag = true;}
    if (ui->recevPortNumEdit->text().isEmpty()) {chatFlag = false;}
        else {chatFlag = true;}
}


void MainWindow::chatDisconnect() {  // disable chat module, prompt user to restart app

    chatSocket->disconnectFromHost();
    ui->warningLabel->setText("Disconnected.\n Must restart application to make new connection.");
//    ui->groupBox_3->setDisabled(true);
//    ui->groupBox_4->setDisabled(true);
//    ui->submitEdit->clear();
//    ui->converseEdit->clear();
    ui->tabWidget->setTabEnabled(1, false);  //set desired tab widget to false
}


void MainWindow::zinMessage() {

    QByteArray datagram;
    QString msg, sendPort, name, addr;

    name = getChatName();
    addr = ui->ipAddrEdit->text();
    sendPort = ui->portNumEdit->text();
    msg = ui->submitEdit->toPlainText();

    if (!msg.isEmpty()) {
        msg = name +" says >> "+ msg;

        QDataStream out(&datagram, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_4);
        out<<msg;

//        chatSocket->writeDatagram(datagram, QHostAddress::LocalHost, sendPort.toInt());
        chatSocket->writeDatagram(datagram, QHostAddress(addr), sendPort.toInt());
        ui->submitEdit->clear();
        ui->submitEdit->setFocus();
        ui->warningLabel->setText("Connection established.");
    }

    else {
        ui->warningLabel->setText("Empty string - not sent.");
    }

}


void MainWindow::processPendingDataGrams() {

    QByteArray mydatagram;

    while (recevSocket->hasPendingDatagrams()) {
        mydatagram.resize(recevSocket->pendingDatagramSize());

        recevSocket->readDatagram(mydatagram.data(), mydatagram.size());
    }

    QString msg;
    QDataStream in(&mydatagram, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_4);
    in>>msg;
    ui->converseEdit->append(msg);
}


void MainWindow::clearChatWindow() {

    ui->converseEdit->clear();
    ui->warningLabel->clear();

}



void MainWindow::monkeyRead() {

    // future function called in constructor to be used for further explanations of software functionality

}


MainWindow::~MainWindow()
{
    aprsSocket->disconnectFromHost();
    chatSocket->disconnectFromHost();
    recevSocket->disconnectFromHost();
    delete ui;
}
