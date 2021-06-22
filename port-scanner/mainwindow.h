#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QLabel>
#include <QtDebug>
#include <QMessageBox>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialog>
#include <QPushButton>
#include <QtGui>
//#include <QRegularExpression>
//#include <QRegularExpressionValidator>
#include <QRegExpValidator>
#include <QtNetwork>
#include <QKeyEvent>  // try event handler again
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

#include "gamble.h"
#include "llfromxy.h"
#include "customlabel.h"

const QString RADIUS="700";  // radius for coverage area
const int APRSPORT = 14580;
const int UAHPORT = 20154;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
//    bool eventFilter(QObject *watched, QEvent *e);

    // getters
    QString getCallSign();  // get from ui
    QString getLatPos();  // get from ui
    QString getLongPos();  // get from ui
    int getPortNumber();
    QString getServerName();
    void clearLineEdits();

    QString getChatName();
    void testEmpty();


    // setters
    void setPortNumber(int);
    void parseAPRS(QString);
    void parseUAH(QString);

    void monkeyRead();


private:
    Ui::MainWindow *ui;
    QLabel* statusLabel;
    Gamble* gam;

    QAction* quitAction;
    QAction* saveAction;
    QAction* helpAction;
    QAction* aboutAction;
    QAction* luckyAction;

    QMenu* fileMenu;
    QMenu* luckyMenu;
    QMenu* helpMenu;

    QTcpSocket* aprsSocket;

    QUdpSocket* chatSocket;
    QUdpSocket* recevSocket;

    // aprs vars
    bool serverFlag;
    int portNumber;
    customLabel *label;

    //chat var
    bool chatFlag;
    int dumInt;

private slots:
    void quitMessage();
    void saveMessage();
    void helpMessage();
    void aboutMessage();
    void luckyMessage();

    void clearPackets();
    void lockAPRSbuttons();
    void unlockAPRSbuttons();
    void writeMessage();
    void processMessage();

    void setServerFlag();
    void otherMapSelect();
    void hsvMapSelect();
    void getCursorPos(QPointF);
    void getLocation();

    void chatDisconnect();
    void startChat();
    void zinMessage();
    void processPendingDataGrams();
    void clearChatWindow();

};

#endif // MAINWINDOW_H
