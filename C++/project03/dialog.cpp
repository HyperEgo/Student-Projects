#include "dialog.h"
#include "ui_dialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtDebug>
#include <QString>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    // set up database object
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("172.21.0.31");
    db.setPort(3300);
    db.setDatabaseName("cpe353");
    db.setUserName("dummy");
    db.setPassword("aeiou1234");

//    db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName("ttable.db");

    if(!db.open()) {
        qDebug()<<db.lastError();
        qDebug()<< "Error: Unable to connect.";
        exit(1);
    }
    else {qDebug()<<"Open table success.";}


    // create and configure scene object
    gscene = new CustomGraphicsScene;
    gscene->setItemIndexMethod(QGraphicsScene::NoIndex);
    gscene->setSceneRect(0, 0, 100, 500);
//    gscene->setSceneRect(QRectF());
    gscene->setBackgroundBrush(Qt::gray);

    // Configure graphics view object
    ui->graphicsView->setScene(gscene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    ui->graphicsView->setMouseTracking(true);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    QVBoxLayout* vLayout = new QVBoxLayout;

    mainLayout->addLayout(vLayout);
    mainLayout->addWidget(ui->graphicsView);
//    vLayout->addLayout(hLayout);

    //fromUtf8(arg)

    QSqlQuery qu;
    QString st = QCoreApplication::arguments().at(1);

    qDebug()<<"inside Dialog cmd line string = "
              <<QCoreApplication::arguments().at(1)<<endl;

    qu.exec("SELECT * FROM "+st);
//    qu.exec("SELECT * FROM colors");
//    qu.exec("SELECT * FROM ttable");
    while (qu.next()) {
        QString str = qu.value(0).toString();
        QLabel* q = new QLabel(str);
        QHBoxLayout* hrz  = new QHBoxLayout;
        hrz->addWidget(q);

        int r = qu.value(1).toInt();
        int g = qu.value(2).toInt();
        int b = qu.value(3).toInt();

        QColor col(r,g,b);
        CustomLabel* cus = new CustomLabel(col);
        hrz->addWidget(cus);
        vLayout->addLayout(hrz);
        vLayout->addStretch(1);
    }


//    QSqlQuery qu;
//    qu.exec("SELECT * FROM ttable");
//    qDebug()<<endl;
//    while (qu.next()) {
//        qDebug()<<"color name = "<<qu.value(0).toString()
//        <<"Red = "<<qu.value(1).toInt()
//        <<"Green = "<<qu.value(2).toInt()
//        <<"Blue = "<<qu.value(3).toInt();
//    }
//    qDebug()<<endl;

}


void Dialog::setArg(QString argv) {

//    str.fromUtf8(argv);
//    qDebug()<<"inside setArg, argv = "<<argv;
    str = argv;
//    qDebug()<<"inside setArg, str = "<<str;
}


QString Dialog::getArg() {

    return str;
}


Dialog::~Dialog()
{
    delete ui;
}
