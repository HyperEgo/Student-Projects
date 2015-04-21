#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // make directory for user pics
    QDir dir;
    dir.mkpath("./pics");

    // keep track of which config ip button was pressed
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
    ui->tabWidget->setTabText(1, "Pictures");
    ui->tabWidget->setTabText(2, "Data");

    ui->groupBox01->setEnabled(false);
    ui->groupBox02->setEnabled(false);

    QString note = "\n1. Configure IP / port.\n\n 2. Dump pictures in to \'pics\' folder.\n\n 3. Press Enter to Send";
    ui->textEdit02->setReadOnly(true);
    ui->textEdit02->setText(note);


    // connect statements
    connect(ui->networkButton01, SIGNAL(clicked()), this, SLOT(setTextFlag()));  // set flag BEFORE calling startConnect()
    connect(ui->networkButton01, SIGNAL(clicked()), this, SLOT(startConnect()));
    connect(ui->closeButton01, SIGNAL(clicked()), this, SLOT(closeSendText()));
    connect(ui->enterButton01, SIGNAL(clicked()), this, SLOT(sendText()));

    connect(ui->networkButton02, SIGNAL(clicked()), this, SLOT(setPicFlag()));
    connect(ui->networkButton02, SIGNAL(clicked()), this, SLOT(startConnect()));
    connect(ui->closeButton02, SIGNAL(clicked()), this, SLOT(closeSendPic()));
    connect(ui->enterButton02, SIGNAL(clicked()), this, SLOT(sendPic()));


//    connect(ui->enterButton02, SIGNAL(clicked()), this, SLOT(loadPics()));  // testing

}



void MainWindow::startConnect() {  // get ip and port number from user; this function is recycled

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
    getIP->setPlaceholderText("127.0.0.1");
    getPort->setPlaceholderText("7777");

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

    getIP->setFocus();  // must be last to get focus working properly

    QObject::connect(okButton, SIGNAL(clicked()), &d, SLOT(close()));

    // force modal behavior
    d.exec();

    // use flags to filter which config button pressed
    QString ip = getIP->text();
    int port = getPort->text().toInt();
    this->setFlagLogic(ip, port);
}


void MainWindow::setTextFlag() {textFlag = true;}

void MainWindow::setPicFlag() {picFlag = true;}

void MainWindow::setDataFlag() {dataFlag = true;}

void MainWindow::disableSendTextButton() {ui->networkButton01->setEnabled(false);}
void MainWindow::enableSendTextButton() {ui->networkButton01->setEnabled(true);}

void MainWindow::disableSendPicButton() {ui->networkButton02->setEnabled(false);}
void MainWindow::enableSendPicButton() {ui->networkButton02->setEnabled(true);}

void MainWindow::setFlagLogic(QString add, int port) {

    if(textFlag==true) {
        ui->groupBox01->setEnabled(true);
        connectText(add, port);
        disableSendText();
    }

    else if(picFlag==true) {
        ui->groupBox02->setEnabled(true);
        connectPic(add, port);
        disableSendPic();
    }

    // reset flags
    textFlag = false;
    picFlag = false;
    dataFlag = false;
}


void MainWindow::connectPic(QString ad, int po) {

    client = new Client;
    client->connectToHost(ad, po);

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
}


void MainWindow::disableSendText() {
    // disable other tabs from configuration

    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setTabEnabled(2, false);
    this->disableSendTextButton();
}


void MainWindow::closeSendText() {
    // re-enable other tabs

    ui->tabWidget->setTabEnabled(1, true);
    ui->tabWidget->setTabEnabled(2, true);
    ui->groupBox01->setEnabled(false);
    this->enableSendTextButton();
}


void MainWindow::connectText(QString ad, int po) {  // send text; connect to host

    client = new Client;
    client->connectToHost(ad, po);
}


void MainWindow::sendText() {  // slot with enter button to send data

    QString msg = ui->textEdit01->toPlainText();
    if(msg.isEmpty()){msg="empty string";}
    ui->textEdit01->clear();
    ui->textEdit01->setFocus();
    QByteArray datagram = msg.toStdString().c_str();
    client->writeData(datagram);
}


void MainWindow::sendPic() {

    luckyPics = new QList<QPixmap*>;

    QDir dir("./pics");  // initiate directory object with path to directory containing .png files
    if (!dir.exists()) {
        qWarning("Lucky image directory not found.");
        return;
    }

    QFileInfoList sList = dir.entryInfoList();  // create file info list object containing paths to .png files
    qDebug()<<"lucky list size = "<<sList.size();  // see list size (number of paths to .png files)

    for(int i=0; i<sList.size(); i++) {  // build pixmap list for images
        QFileInfo fileInfo = sList.at(i);
        QPixmap *pix = new QPixmap(fileInfo.filePath());
        qDebug()<<fileInfo.filePath();
        luckyPics->push_back(pix);
    }

    if(luckyPics->isEmpty()) {qDebug()<<"luckyPic list empty";}
    else {qDebug()<<"luckyPic obj size = "<<luckyPics->size();}

    QPixmap *pixy = new QPixmap;
    int t = (qrand()+luckyPics->size()) % luckyPics->size();  // calculate random number from pic container
    pixy = luckyPics->operator[](t);  // acquire random image

    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    pixy->save(&buffer, "PNG");
    client->writeData(bytes);

}



bool MainWindow::eventFilter(QObject *watched, QEvent *e) {  // event filter for user press enter key

    if (watched==this && e->type()==QEvent::KeyPress) {
        QKeyEvent *ke = (QKeyEvent*)e;

        if (ke->key()==Qt::Key_Enter || ke->key()==Qt::Key_Return) {
            this->sendText();
            return true;
        }

    }
    return MainWindow::eventFilter(watched, e);
}



MainWindow::~MainWindow()
{
    delete ui;
}
