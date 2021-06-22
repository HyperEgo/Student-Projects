#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    server = new Server;

    // set flags for socket initialization
    textFlag = false;
    picFlag = false;
    dataFlag = false;


    // set fixed window size
    QSize s = this->size();
    this->statusBar()->setSizeGripEnabled(false);
    this->setFixedSize(s);


    // center main window on screen at execution
    QRect position = this->frameGeometry();
    position.moveCenter(QDesktopWidget().availableGeometry().center());
    move(position.topLeft());


    // intialize tabs & ui components
    ui->tabWidget->widget(0)->setFocus();
    ui->tabWidget->setCurrentWidget(0);
    ui->tabWidget->setTabText(0, "Text");
    ui->tabWidget->setTabText(1, "Picture");
    ui->tabWidget->setTabText(2, "Data");

    ui->textEdit01->setReadOnly(true);
    ui->closeButton01->setEnabled(false);


    // connect statements
    connect(ui->networkButton01, SIGNAL(clicked()), this, SLOT(setTextFlag()));  // set flag BEFORE calling startConnect()
    connect(ui->networkButton01, SIGNAL(clicked()), this, SLOT(startConnect()));
    connect(ui->closeButton01, SIGNAL(clicked()), this, SLOT(enableTabs01()));

    connect(ui->networkButton02, SIGNAL(clicked()), this, SLOT(setPicFlag()));
    connect(ui->networkButton02, SIGNAL(clicked()), this, SLOT(startConnect()));
    connect(ui->closeButton02, SIGNAL(clicked()), this, SLOT(closeSendPic()));

    connect(server, SIGNAL(dataReceived(QByteArray)), SLOT(getData(QByteArray)));
    connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(clearTextEdit()));

}


void MainWindow::startConnect() {  // get ip and port number from user

    // pop-up box
    QDialog d;
    d.setWindowTitle("Networking");
    QVBoxLayout *main = new QVBoxLayout;
    QHBoxLayout *horiz01 = new QHBoxLayout;
    QHBoxLayout *horiz02 = new QHBoxLayout;
    QPushButton *okButton = new QPushButton("Ok");
    QLabel *label01 = new QLabel("IP");
    QLabel *label02 = new QLabel("Port");
    QLineEdit *getIP = new QLineEdit;
    QLineEdit *getPort = new QLineEdit;
    getIP->setPlaceholderText("10.0.0.1");
    getPort->setPlaceholderText("5555");

    // ip address validator
    QRegExpValidator *v = new QRegExpValidator;
    QRegExp rx("((1{0,1}[0-9]{0,2}|2[0-4]{1,1}[0-9]{1,1}|25[0-5]{1,1})\\.){3,3}(1{0,1}[0-9]{0,2}|2[0-4]{1,1}[0-9]{1,1}|25[0-5]{1,1})");
    v->setRegExp(rx);
    getIP->setValidator(v);

    // port number validator
    QIntValidator* intVal = new QIntValidator(0, 65535, getPort);
    getPort->setValidator(intVal);

    d.setLayout(main);

    main->addSpacing(25);
    main->addLayout(horiz01);
    main->addSpacing(25);
    main->addLayout(horiz02);
    main->addSpacing(25);
    main->addWidget(okButton);

    horiz01->addWidget(label01);
    horiz01->addSpacing(15);
    horiz01->addWidget(getIP);

    horiz02->addWidget(label02);
    horiz02->addSpacing(15);
    horiz02->addWidget(getPort);

    getIP->setFocus();  // set last

    QObject::connect(okButton, SIGNAL(clicked()), &d, SLOT(close()));

    // force modal behavior
    d.exec();

    // set flag filter
    QString ip = getIP->text();
    int port = getPort->text().toInt();
    this->setFlagLogic(ip, port);
}


void MainWindow::setTextFlag() {textFlag = true;}

void MainWindow::setPicFlag() {picFlag = true;}

void MainWindow::setDataFlag() {dataFlag = true;}

void MainWindow::disableTextButton() {ui->networkButton01->setEnabled(false);}
void MainWindow::enableTextButton() {ui->networkButton01->setEnabled(true);}

void MainWindow::disableSendPicButton() {ui->networkButton02->setEnabled(false);}
void MainWindow::enableSendPicButton() {ui->networkButton02->setEnabled(true);}

void MainWindow::disableTabs01() {
    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setTabEnabled(2, false);
    ui->closeButton01->setEnabled(true);
    this->disableTextButton();
}


void MainWindow::enableTabs01(){
    ui->tabWidget->setTabEnabled(1, true);
    ui->tabWidget->setTabEnabled(2, true);
    ui->closeButton01->setEnabled(false);
    this->enableTextButton();
    textFlag = false;
    picFlag = false;
    dataFlag = false;
}


void MainWindow::disableSendPic() {

    ui->tabWidget->setTabEnabled(0, false);
    ui->tabWidget->setTabEnabled(2, false);
    this->disableSendPicButton();
}


void MainWindow::closeSendPic() {

    ui->tabWidget->setTabEnabled(0, true);
    ui->tabWidget->setTabEnabled(2, true);
    ui->groupBox02->setEnabled(false);
    this->enableSendPicButton();
    textFlag = false;
    picFlag = false;
    dataFlag = false;
}


void MainWindow::setFlagLogic(QString add, int port) {
    // determine which config button is clicked

    if(textFlag==true) {
        connectText(add, port);
        disableTabs01();
    }

    else if(picFlag==true) {
        ui->groupBox02->setEnabled(true);
        connectPic(add, port);
        disableSendPic();
    }

    // reset flags
//    textFlag = false;
//    picFlag = false;
//    dataFlag = false;
}


void MainWindow::connectText(QString ad, int po) {  // send text; connect to host

    server->connectListen(ad, po);

}


void MainWindow::connectPic(QString ad, int po) {  // send text; connect to host

    server->connectListen(ad, po);

}


void MainWindow::getData(QByteArray ba) {  // get data from Server obj

        if(textFlag==true) {
            QString msg(ba);
            ui->textEdit01->append(msg);
        }
        else {
            QPixmap p;
        //    QByteArray pData;
            // fill array with image
            if(p.loadFromData(ba,"PNG"))
            {
               // do something with pixmap
            ui->displayPic->setPixmap(p);
            }
        }

}


void MainWindow::clearTextEdit() {ui->textEdit01->clear();}


MainWindow::~MainWindow()
{
    delete ui;
}
