#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QtNetwork>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QtDebug>
#include <QMessageBox>
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QtGui>
#include <QTextStream>
#include <QObject>
#include <QIcon>

const int COMPORT = 5555;
const int RECVPORT = 4444;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void introMessage();
    void lockButtons();
    void unlockButtons();

    void setUserIP(QString str);
    void setUserPort(int h);

    QString getUserIP();
    int getUserPort();


private:
    Ui::MainWindow *ui;

    QUdpSocket* sendSocket;
    QUdpSocket* recvSocket;
    QString userIP;
    int userPort;



private slots:
    void getConnect();
    void getCancel();
    void zinMessage();
    void unlockB();
    void processPendingDataGrams();

};

#endif // MAINWINDOW_H
