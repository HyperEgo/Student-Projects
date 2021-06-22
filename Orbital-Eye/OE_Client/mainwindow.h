#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialog>
#include <QDial>
#include <QSlider>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QtGui>
#include <QAction>
#include <QMenu>
#include <QTextEdit>
#include <QtDebug>
#include <QString>
#include <QtSql>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QProgressBar>

#include "satdata.h"
#include "customlabel.h"
#include "llfromxy.h"
#include "animatedialog.h"
#include "animated2.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QPointF getMapCoordinates();
    int getAzimuthValue();
    int getElevationValue();

private:
    Ui::MainWindow *ui;
    customLabel *label;
    QPointF userMapCoordinates;
    int userAzimuthValue;
    int userElevationValue;
    bool connectToDatabase();
    // SQL database
    QSqlDatabase *db;
    QList<satData> satList;
    QMessageBox msgBox;
    QMessageBox help_msgBox;
    QMessageBox about_msgBox;
    AnimateDialog *ad;
    AnimateD2 *az;


private slots:
    void getLocation();
    void getBoresight();
    void getAnimate();
    void getTable();
    void getCursorPos(QPointF);
    void message();
    void testSlot(int);
    void helpInfo();
    void aboutInfo();


};

#endif // MAINWINDOW_H
