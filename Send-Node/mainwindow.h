#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QtNetwork>
#include <QTcpSocket>
#include <QFileDialog>
#include <QFile>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialog>
#include <QPushButton>
#include <QtGui>
#include <QLineEdit>
//#include <QRegularExpressionValidator>
//#include <QRegularExpression>
#include <QtDebug>
#include <QKeyEvent>
#include <QByteArray>
#include <QList>
#include <QDir>

#include "client.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT    

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool eventFilter(QObject *watched, QEvent *e);
    void connectText(QString, int);
    void connectPic(QString, int);

    void disableSendText();
    void disableSendTextButton();
    void enableSendTextButton();

    void disableSendPicButton();
    void enableSendPicButton();
    void disableSendPic();


private:
    Ui::MainWindow *ui;

    bool textFlag, picFlag, dataFlag;

    Client* client;
    QList<QPixmap*> *luckyPics;
//    QDir dir;




private slots:
    void startConnect();
    void setTextFlag();
    void setPicFlag();
    void setDataFlag();
    void setFlagLogic(QString, int);

    void sendText();
    void closeSendText();

    void sendPic();
    void closeSendPic();

//    void loadPics();


};

#endif // MAINWINDOW_H
