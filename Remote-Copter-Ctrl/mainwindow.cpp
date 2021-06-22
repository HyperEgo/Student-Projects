#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    ui->label_2->setStyleSheet("background-color: white;");
    ui->label_2->setPixmap(QPixmap(":/images/tron_console02").scaled(ui->label_2->size()));

    sendSocket = new QUdpSocket(this);
    recvSocket = new QUdpSocket(this);
    this->recvSocket->bind(RECVPORT);
    qDebug()<<"Receive port [UDP]: = "+QString::number(RECVPORT);

    introMessage();
    lockButtons();

    // set button style-sheets
//    ui->up1->setStyleSheet("background-color: yellow;"
//                           "border-style: outset;"
//                           "border-width: 2px;"
//                           "border-radius: 7px;"
//                           "border-color: beige;"
//                           "padding: 5px;");

    // connect statements
    connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(getConnect()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(getCancel()));
    connect(recvSocket, SIGNAL(readyRead()), this, SLOT(processPendingDataGrams()));

    connect(ui->up1, SIGNAL(clicked()), this, SLOT(zinMessage()));
    connect(ui->up2, SIGNAL(clicked()), this, SLOT(zinMessage()));
    connect(ui->down1, SIGNAL(clicked()), this, SLOT(zinMessage()));
    connect(ui->down2, SIGNAL(clicked()), this, SLOT(zinMessage()));
    connect(ui->right1, SIGNAL(clicked()), this, SLOT(zinMessage()));
    connect(ui->right2, SIGNAL(clicked()), this, SLOT(zinMessage()));
    connect(ui->left1, SIGNAL(clicked()), this, SLOT(zinMessage()));
    connect(ui->left2, SIGNAL(clicked()), this, SLOT(zinMessage()));
}


void MainWindow::introMessage() {

    QDialog dia;
    QMessageBox intro;

    intro.information(&dia,"Remote Copter Controls","Welcome to Remote Copter Controls.\n\n"
                            "Senior Design Project:\n"
                            "Alex Harmon, Jacob Walsh and Fleance Collins\n\n"
                            "1.  Connect to Copter, first.\n"
                            "2.  Configure IP v4 address and port number.\n"
                            "3.  Use directional buttons for steering copter.\n\n",
                            QMessageBox::Close|QMessageBox::NoButton);
}


void MainWindow::zinMessage() {

    QByteArray datagram;
    QString msg, addr;
    int sendPort;

    addr = this->getUserIP();
    sendPort = this->getUserPort();

    QDataStream out(&datagram, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_4);

    QPushButton* bu = qobject_cast<QPushButton*>(sender());

    // get button id of sender
    if("up1" == bu->accessibleName()) {
        msg = "up1";
        out<<msg;
    }

    if("up2" == bu->accessibleName()) {
        msg = "up2";
        out<<msg;
    }

    if("left1" == bu->accessibleName()) {
        msg = "left1";
        out<<msg;
    }

    if("left2" == bu->accessibleName()) {
        msg = "left2";
        out<<msg;
    }

    if("right1" == bu->accessibleName()) {
        msg = "right1";
        out<<msg;
    }

    if("right2" == bu->accessibleName()) {
        msg = "right2";
        out<<msg;
    }

    if("down1" == bu->accessibleName()) {
        msg = "down1";
        out<<msg;
    }

    if("down2" == bu->accessibleName()) {
        msg = "down2";
        out<<msg;
    }

    // default string
    msg = "default";
    out<<msg;

//    sendSocket->writeDatagram(datagram, QHostAddress(addr), COMPORT);
//    sendSocket->writeDatagram(datagram, QHostAddress("146.229.163.213"), 5555);
    sendSocket->writeDatagram(datagram, QHostAddress(addr), sendPort);

    qDebug()<<"zinMessage funct ip: = "+addr;
    qDebug()<<"zinMessage funct port: = "+QString::number(sendPort);

}


void MainWindow::getConnect() {

//    bool cFlag1 = false;
//    bool cFlag2 = false;

    this->ui->connectButton->setEnabled(false);

    // get ip addr and port number from user with validation
    QDialog d;
    d.resize(300,250);
//    d.setWindowTitle("Connect");
    QVBoxLayout *mainlayout = new QVBoxLayout;
    QHBoxLayout *horizlayout = new QHBoxLayout;
    QPushButton *okButton = new QPushButton("Ok");
    QPushButton *closeButton = new QPushButton("Close");
    QLabel *label1 = new QLabel("Enter IP v4 Address");
    QLabel *label2 = new QLabel("Enter Port Number");
    QLineEdit *ipAddr = new QLineEdit;
    QLineEdit *portNum = new QLineEdit;

    // set dialog style-sheets; color distinction
    d.setStyleSheet("background-color: cyan;"
                    "border: 1px solid darkGray");
    label1->setStyleSheet("background-color: white;");
    label2->setStyleSheet("background-color: white;");
    ipAddr->setStyleSheet("background-color: white;");
    portNum->setStyleSheet("background-color: white;");
    okButton->setStyleSheet("background-color: white;");
    closeButton->setStyleSheet("background-color: white;");

    // position dialog
    QRect position = d.frameGeometry();
    position.moveCenter(QDesktopWidget().availableGeometry().center());
    d.move(position.topLeft());

    // validators
    QRegExpValidator *v = new QRegExpValidator(this);
    QRegExp rx("((1{0,1}[0-9]{0,2}|2[0-4]{1,1}[0-9]{1,1}|25[0-5]{1,1})\\.){3,3}(1{0,1}[0-9]{0,2}|2[0-4]{1,1}[0-9]{1,1}|25[0-5]{1,1})");
    v->setRegExp(rx);
    ipAddr->setValidator(v);
    ipAddr->setInputMethodHints(Qt::ImhDigitsOnly);

    QIntValidator* portVal = new QIntValidator(0, 65535, portNum);
    portNum->setValidator(portVal);
    portNum->setInputMethodHints(Qt::ImhDigitsOnly);

    // text hints
    ipAddr->setPlaceholderText("127.0.0.1");
    portNum->setPlaceholderText("5555");

    d.setLayout(mainlayout);
    mainlayout->addWidget(label1);
    mainlayout->addSpacing(7);
    mainlayout->addWidget(ipAddr);
    mainlayout->addSpacing(25);
    mainlayout->addWidget(label2);
    mainlayout->addSpacing(7);
    mainlayout->addWidget(portNum);
    mainlayout->addSpacing(20);
    mainlayout->addLayout(horizlayout);

    horizlayout->addWidget(okButton);
    horizlayout->addSpacing(10);
    horizlayout->addWidget(closeButton);

    QObject::connect(okButton, SIGNAL(clicked()), &d, SLOT(close()));
    QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(unlockB()));

    QObject::connect(closeButton, SIGNAL(clicked()), &d, SLOT(close()));
    QObject::connect(closeButton, SIGNAL(clicked()), this, SLOT(getCancel()));

    // force modal behavior
    d.exec();

    this->setUserIP(ipAddr->text());
    this->setUserPort(portNum->text().toInt());

    qDebug()<<"Connect funct ip: = "+ipAddr->text();
    qDebug()<<"Connect funct port: = "+portNum->text();
}


void MainWindow::unlockB() {

    // slot to unlock button
    this->unlockButtons();
}


void MainWindow::getCancel() {

    // cancel ip addr and port inputs
    this->ui->connectButton->setEnabled(true);
    this->lockButtons();
}


void MainWindow::processPendingDataGrams() {  // write to main window label for receive communications

    QByteArray mydatagram;

    while (recvSocket->hasPendingDatagrams()) {
        mydatagram.resize(recvSocket->pendingDatagramSize());
        recvSocket->readDatagram(mydatagram.data(), mydatagram.size());
    }

    QString msg;  // declare additional receive variables *HERE*
    QDataStream in(&mydatagram, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_4);
    in>>msg;  // can receive data in to string, icon or pixmap variables *HERE*
    ui->label_2->setText(msg);  // can set string, icon, or pixmap images to label *HERE*
}


void MainWindow::lockButtons() {

    QIcon icon = QIcon(":/images/green_lock.png");

    ui->up1->setIcon(icon);
    ui->up1->setIconSize(ui->up1->size());
    ui->up2->setIcon(icon);
    ui->up2->setIconSize(ui->up2->size());
    ui->left1->setIcon(icon);
    ui->left1->setIconSize(ui->left1->size());
    ui->left2->setIcon(icon);
    ui->left2->setIconSize(ui->left2->size());
    ui->right1->setIcon(icon);
    ui->right1->setIconSize(ui->right1->size());
    ui->right2->setIcon(icon);
    ui->right2->setIconSize(ui->right2->size());
    ui->down1->setIcon(icon);
    ui->down1->setIconSize(ui->down1->size());
    ui->down2->setIcon(icon);
    ui->down2->setIconSize(ui->down2->size());

    ui->up1->setEnabled(false);
    ui->up2->setEnabled(false);
    ui->left1->setEnabled(false);
    ui->left2->setEnabled(false);
    ui->right1->setEnabled(false);
    ui->right2->setEnabled(false);
    ui->down1->setEnabled(false);
    ui->down2->setEnabled(false);
}


void MainWindow::unlockButtons() {

    QIcon icon = QIcon();
    icon.addPixmap(QPixmap(":/images/arrow_up_red.png"));
    ui->up1->setIcon(icon);
    ui->up1->setIconSize(ui->up1->size());

    icon.addPixmap(QPixmap(":/images/arrow_up_red.png"));
    ui->up2->setIcon(icon);
    ui->up2->setIconSize(ui->up1->size());

    icon.addPixmap(QPixmap(":/images/arrow_left_red.png"));
    ui->left1->setIcon(icon);
    ui->left1->setIconSize(ui->left1->size());

    icon.addPixmap(QPixmap(":/images/arrow_left_red.png"));
    ui->left2->setIcon(icon);
    ui->left2->setIconSize(ui->left2->size());

    icon.addPixmap(QPixmap(":/images/arrow_right_red.png"));
    ui->right1->setIcon(icon);
    ui->right1->setIconSize(ui->right1->size());

    icon.addPixmap(QPixmap(":/images/arrow_right_red.png"));
    ui->right2->setIcon(icon);
    ui->right2->setIconSize(ui->right2->size());

    icon.addPixmap(QPixmap(":/images/arrow_down_red.png"));
    ui->down1->setIcon(icon);
    ui->down1->setIconSize(ui->down1->size());

    icon.addPixmap(QPixmap(":/images/arrow_down_red.png"));
    ui->down2->setIcon(icon);
    ui->down2->setIconSize(ui->down2->size());

    ui->up1->setEnabled(true);
    ui->up2->setEnabled(true);
    ui->left1->setEnabled(true);
    ui->left2->setEnabled(true);
    ui->right1->setEnabled(true);
    ui->right2->setEnabled(true);
    ui->down1->setEnabled(true);
    ui->down2->setEnabled(true);
}

void MainWindow::setUserIP(QString str) {this->userIP = str;}

void MainWindow::setUserPort(int h) {this->userPort = h;}

QString MainWindow::getUserIP(){  // error check ip address

    if(this->userIP.isEmpty()){return "127.0.0.1";}
    else {return this->userIP;}
}

int MainWindow::getUserPort(){  // error check port number = 0

    if(this->userPort==0){return COMPORT;}
    else {return this->userPort;}
}


MainWindow::~MainWindow()
{
    sendSocket->disconnectFromHost();
    recvSocket->disconnectFromHost();
    delete ui;
}
