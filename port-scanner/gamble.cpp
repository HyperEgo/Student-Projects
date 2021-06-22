#include "gamble.h"
#include "ui_gamble.h"

Gamble::Gamble(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Gamble)
{
    ui->setupUi(this);
    this->installEventFilter(this);

    setWindowTitle("Lucky");

    loadPics();

    // set fixed window size
    QSize s = this->size();
    this->setFixedSize(s);


    // ui widgets initialize
    ui->statusLabel->clear();
    ui->dial1->setText("$");
    ui->dial2->setText("$");
    ui->dial3->setText("$");
    ui->spinOut->clear();
    ui->winOut->clear();
    ui->spinButton->setFocus();

    ui->dial1->setReadOnly(true);
    ui->dial2->setReadOnly(true);
    ui->dial3->setReadOnly(true);

    flag = false;

    ui->titleLogo->setPixmap(QPixmap(":/images/ccastles.png").scaled(ui->titleLogo->size(),Qt::KeepAspectRatioByExpanding));
    ui->bottomLogo->setPixmap(QPixmap(":/images/pot_gold02.png").scaled(ui->bottomLogo->size(),Qt::KeepAspectRatio));
    ui->bottomLogo->hide();

    // initial gamble logos
    ui->leftLogo->setPixmap(QPixmap(":/images/lep_mad.png"));
    ui->rightLogo->setPixmap(QPixmap(":/images/monkey_grim.png"));
    ui->centerLogo->setPixmap(QPixmap(":/images/monkey_deadpool.png"));


//    // return pink blonde pics, after keystroke
//    ui->leftLogo->setPixmap(QPixmap(":/images/blonde_back02.png"));
//    ui->rightLogo->setPixmap(QPixmap(":/images/pink_blonde_flip.jpg"));
//    ui->centerLogo->setPixmap(QPixmap(":/images/pink_blonde.jpg"));

//    ui->titleButton->setStyleSheet("border-image: url(:images/ccastles.png) 0 -75 0 0;");
//    ui->titleButton->setStyleSheet("border-image: url(:images/ccastles.png) 0 0 0 0;");


    //connect stmts
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));  // close button, testing?
    connect(ui->infoButton, SIGNAL(clicked()), this, SLOT(infoMessage()));
    connect(ui->spinButton, SIGNAL(clicked()), this, SLOT(processSpin()));
}


void Gamble::infoMessage() {

    // before exiting game display game loose pic inside coded QDialog; dialog ok button closes pic (label) and gamble object!

    QDialog dialog1;
    QMessageBox help_msgBox;

    help_msgBox.information(&dialog1,"Lucky Info","Simple Rules.\n\n"
                            "Three in a row wins, plus get a picture.\n"
                            "Two in a row wins.\n"
                            "Snake eyes loses.\n\n"

                            "Get to 13 wins for grand prize.\n"
                            "Good luck.\n",
                            QMessageBox::Close|QMessageBox::NoButton);
}


void Gamble::processSpin() {

    // three numbers in a row win
    // two numbers in a row win
    // snake eyes loose
    // get to 13 wins without hitting snake eyes

    ui->dial1->setStyleSheet("");
    ui->dial2->setStyleSheet("");
    ui->dial3->setStyleSheet("");

    qsrand(qrand());

    int one = (qrand()+MODULUS) % MODULUS;
    int two = (qrand()+MODULUS) % MODULUS;
    int three = (qrand()+MODULUS) % MODULUS;

    ui->dial1->setText(QString::number(one));
    ui->dial2->setText(QString::number(two));
    ui->dial3->setText(QString::number(three));

    int spins=ui->spinOut->text().toInt()+1;
    ui->spinOut->setText(QString::number(spins));

    // three in a row
    if((one==two) && (two==three)) {
        ui->statusLabel->setText("Winner");
        int wins = ui->winOut->text().toInt()+1;
        ui->winOut->setText(QString::number(wins));

        ui->dial1->setStyleSheet("border: 4px solid red");
        ui->dial2->setStyleSheet("border: 4px solid red");
        ui->dial3->setStyleSheet("border: 4px solid red");

        // get a picture
        QDialog g;
        g.setWindowTitle("Three In A Row - You Win");
        QVBoxLayout *mainlayoutz = new QVBoxLayout;
        QPixmap *pixf = new QPixmap;
        pixf = getPicture();
        QLabel *picLabelz = new QLabel;
        QPushButton *closeButtonz = new QPushButton("Close");

        g.setLayout(mainlayoutz);
        picLabelz->setPixmap(*pixf);
        mainlayoutz->addWidget(picLabelz);
        mainlayoutz->addSpacing(15);
        mainlayoutz->addWidget(closeButtonz);

        QObject::connect(closeButtonz, SIGNAL(clicked()), &g, SLOT(close()));
//        QObject::connect(closeButtonz, SIGNAL(clicked()), this, SLOT(close()));

        // force modal behavior
        g.exec();

    }

    // two in a row
    else if (one==two && one!=0) {
        ui->dial1->setStyleSheet("border: 4px solid blue");
        ui->dial2->setStyleSheet("border: 4px solid blue");
        int wins = ui->winOut->text().toInt()+1;
        ui->winOut->setText(QString::number(wins));
        ui->statusLabel->setText("Winner");
    }

    else if (two==three && two!=0) {
        ui->dial2->setStyleSheet("border: 4px solid blue");
        ui->dial3->setStyleSheet("border: 4px solid blue");
        int wins = ui->winOut->text().toInt()+1;
        ui->winOut->setText(QString::number(wins));
        ui->statusLabel->setText("Winner");
    }

    else if (one==three && three!=0) {
        ui->dial1->setStyleSheet("border: 4px solid blue");
        ui->dial3->setStyleSheet("border: 4px solid blue");
        int wins = ui->winOut->text().toInt()+1;
        ui->winOut->setText(QString::number(wins));
        ui->statusLabel->setText("Winner");
    }

    else {
        ui->statusLabel->setText("Try Again");

        ui->dial1->setStyleSheet("");
        ui->dial2->setStyleSheet("");
        ui->dial3->setStyleSheet("");
    }

    //snake eyes
    if ((one==0 && two==0 && three!=0) || (two==0 && three==0 && one!=0) || (one==0 && three==0 && two!=0)) {

        if(one==0 && two==0 && three!=0){
            ui->dial1->setStyleSheet("border: 4px solid green");
            ui->dial2->setStyleSheet("border: 4px solid green");
        }
        else if(two==0 && three==0 && one!=0) {
            ui->dial2->setStyleSheet("border: 4px solid green");
            ui->dial3->setStyleSheet("border: 4px solid green");
        }
        else if(one==0 && three==0 && two!=0) {
            ui->dial1->setStyleSheet("border: 4px solid green");
            ui->dial3->setStyleSheet("border: 4px solid green");
        }

        ui->statusLabel->setText("Loser");

        QDialog d;
        d.setWindowTitle("Snake Eyes - You Lose");
        QVBoxLayout *mainlayout = new QVBoxLayout;
        QPixmap pix(":/images/monkey_cry.jpg");
        QLabel *picLabel = new QLabel;
        QPushButton *closeButton = new QPushButton("Close");

        d.setLayout(mainlayout);
        picLabel->setPixmap(pix);
        mainlayout->addWidget(picLabel);
        mainlayout->addSpacing(15);
        mainlayout->addWidget(closeButton);

        QObject::connect(closeButton, SIGNAL(clicked()), &d, SLOT(close()));
        QObject::connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
        if(d.close()){this->close();} // account for abrupt closing

        // force modal behavior
        d.exec();
    }

    // pot of gold >= 7 wins
    if ((ui->winOut->text().toInt()) >= 7) {ui->bottomLogo->show();}

    // grand prize
    if ((ui->winOut->text().toInt()) >= 13) {


        QDialog g;
        g.setWindowTitle("Grand Prize - You Win");
        QVBoxLayout *mainlayoutz = new QVBoxLayout;
        QHBoxLayout *horiz = new QHBoxLayout;
        QPixmap *pixf = new QPixmap;
        QPixmap *pixt = new QPixmap;
        pixf = getPicture();
        pixt = getPicture();
        QLabel *picLabelz = new QLabel;
        QLabel *picLabely = new QLabel;
        QPushButton *closeButtonz = new QPushButton("Close");

        g.setLayout(mainlayoutz);
        mainlayoutz->addLayout(horiz);
        picLabelz->setPixmap(*pixf);
        picLabely->setPixmap(*pixt);
        horiz->addWidget(picLabelz);
        horiz->addSpacing(15);
        horiz->addWidget(picLabely);
        mainlayoutz->addSpacing(15);
        mainlayoutz->addWidget(closeButtonz);

        QObject::connect(closeButtonz, SIGNAL(clicked()), &g, SLOT(close()));
//        QObject::connect(closeButtonz, SIGNAL(clicked()), this, SLOT(close()));
        QObject::connect(closeButtonz, SIGNAL(clicked()), this, SLOT(resetGamble()));
        if(g.close()) {this->resetGamble();}  // reset after abrupt closing

        // force modal behavior
        g.exec();

    }

}


QPixmap* Gamble::getPicture() {

    QPixmap *pixy = new QPixmap;

    if(getFlag()) {
//        int t = qrand() % luckyPics->size();  // calculate random number from pic container
        int t = (qrand()+luckyPics->size()) % luckyPics->size();  // calculate random number from pic container
        pixy = luckyPics->operator[](t);  // acquire random image
        return pixy;
    }

    else {
//        int v = qrand() % bunnyPics->size();  // calculate random number from pic container
        int v = (qrand()+bunnyPics->size()) % bunnyPics->size();  // calculate random number from pic container
        pixy = bunnyPics->operator[](v);  // acquire random image
        return pixy;
    }
}



void Gamble::loadPics() {

    luckyPics = new QList<QPixmap*>;
    bunnyPics = new QList<QPixmap*>;


    //**************  lucky pics  ********************
    QDir dir(":/images/lucky/");  // initiate directory object with path to directory containing .png files
    if (!dir.exists()) {
        qWarning("Lucky image directory not found.");
        return;
    }

    QFileInfoList sList = dir.entryInfoList();  // create file info list object containing paths to .png files
//    qDebug()<<"lucky list size = "<<sList.size();  // see list size (number of paths to .png files)

    for(int i=0; i<sList.size(); i++) {  // build pixmap list for images
        QFileInfo fileInfo = sList.at(i);
        QPixmap *pix = new QPixmap(fileInfo.filePath());
//        qDebug()<<fileInfo.filePath();
        luckyPics->push_back(pix);
    }

    if(luckyPics->isEmpty()) {qDebug()<<"luckyPic list empty";}
//    else {qDebug()<<"luckyPic obj size = "<<luckyPics->size();}



    //*************  bunny pics   ****************
    QDir dir1(":/images/bunny/");  // initiate directory object with path to directory containing .png files
    if (!dir1.exists()) {
        qWarning("Bunny image directory not found.");
        return;
    }

    QFileInfoList zList = dir1.entryInfoList();  // create file info list object containing paths to .png files
//    qDebug()<<"bunny list size = "<<zList.size();  // see list size (number of paths to .png files)

    for(int j=0; j<zList.size(); j++) {  // build pixmap list for images
        QFileInfo fileInfo1 = zList.at(j);
        QPixmap *pix1 = new QPixmap(fileInfo1.filePath());
//        qDebug()<<fileInfo1.filePath();
        bunnyPics->push_back(pix1);
    }
    if(bunnyPics->isEmpty()) {qDebug()<<"bunnyPic list empty";}
//    else {qDebug()<<"bunnyPic obj size = "<<bunnyPics->size();}

}


void Gamble::setFlag(bool flg) {

    flag = flg;
}



bool Gamble::getFlag() {

    return flag;
}



bool Gamble::eventFilter(QObject *watched, QEvent *e) {

    if (watched==this && e->type()==QEvent::KeyPress) {
//       QKeyEvent *ke = static_cast<QKeyEvent*>(e);
    QKeyEvent *ke = (QKeyEvent*)e;

//        if (ke->key()== Qt::Key_Enter || ke->key()==Qt::Key_Return) {
//        if (ke->key()== Qt::Key_F1 || ke->key()==Qt::Key_F2) {
//        if (ke->matches(QKeySequence::Copy)) {
//        if (ke->modifiers().testFlag(Qt::ShiftModifier) && ke->modifiers().testFlag(Qt::ControlModifier)) {
        if (ke->modifiers().testFlag(Qt::ControlModifier) && ke->modifiers().testFlag(Qt::AltModifier)) {
            if (ke->key()==Qt::Key_1) {

                // pink blonde pics - after keystroke
                ui->leftLogo->setPixmap(QPixmap(":/images/blonde_back02.png"));
                ui->rightLogo->setPixmap(QPixmap(":/images/pink_blonde_flip.jpg"));
                ui->centerLogo->setPixmap(QPixmap(":/images/pink_blonde.jpg"));
                setFlag(true);
                return true;
            }
        }

        else if (ke->key()==Qt::Key_1) {

            // restore initial logos
            ui->leftLogo->setPixmap(QPixmap(":/images/lep_mad.png"));
            ui->rightLogo->setPixmap(QPixmap(":/images/monkey_grim.png"));
            ui->centerLogo->setPixmap(QPixmap(":/images/monkey_deadpool.png"));
            setFlag(false);
            return false;
        }

    }
    return QWidget::eventFilter(watched, e);
}


void Gamble::resetGamble() {

    // reset widgets
    ui->bottomLogo->hide();
    ui->statusLabel->clear();
    ui->dial1->setText("$");
    ui->dial2->setText("$");
    ui->dial3->setText("$");
    ui->spinOut->clear();
    ui->winOut->clear();
    ui->spinButton->setFocus();

    ui->dial1->setStyleSheet("");
    ui->dial2->setStyleSheet("");
    ui->dial3->setStyleSheet("");

    setFlag(false);
    ui->leftLogo->setPixmap(QPixmap(":/images/lep_mad.png"));
    ui->rightLogo->setPixmap(QPixmap(":/images/monkey_grim.png"));
    ui->centerLogo->setPixmap(QPixmap(":/images/monkey_deadpool.png"));
}




Gamble::~Gamble() {

    delete luckyPics;
    delete bunnyPics;
    delete ui;
}
