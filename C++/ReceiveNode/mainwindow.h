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
#include <QtDebug>

#include "server.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void connectText(QString, int);
    void disableTextButton();
    void enableTextButton();
    void disableTabs01();
    void setFlagLogic(QString, int);

    void disableSendPicButton();
    void enableSendPicButton();
    void disableSendPic();
    void connectPic(QString, int);


private:
    Ui::MainWindow *ui;

    bool textFlag, picFlag, dataFlag;

    Server* server;


private slots:
    void startConnect();
    void setTextFlag();
    void setPicFlag();
    void setDataFlag();
    void enableTabs01();

    void getData(QByteArray);
    void clearTextEdit();

    void closeSendPic();

};

#endif // MAINWINDOW_H
