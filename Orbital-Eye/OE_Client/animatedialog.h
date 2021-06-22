#ifndef ANIMATEDIALOG_H
#define ANIMATEDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QTime>
#include <QDebug>
#include <QList>
#include <QPixmap>
#include <QGraphicsScene>
#include <QPen>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include "satdata.h"
#include "satellitemanager.h"

namespace Ui {
class AnimateDialog;
}

class AnimateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AnimateDialog(QWidget *parent = 0);
    ~AnimateDialog();
    void setAllParameters(QList<satData> *aList, double lat, double lon, double boreAZ, double boreEL, double fovDegrees, int radiusInPixels);

private:
    Ui::AnimateDialog *ui;
    QWidget *theParent;
    QGraphicsScene *myScene;
    QTimer *myTimer;
    QList<satData> *satList;
    SatelliteManager *manager;
    QList<Satellite> *satellites;
    time_t unixTimeClock;
    QColor getColor(int id);
    QPen *pen;
    QBrush *brush;
    QPixmap *aMap;
    QString numSatsPrefix;
    QString numSatsFull;
    qint64 userTime1970;
    QTime userTime;
    QDate userDate;
    bool timeInitialized;

private slots:
    void startButtonActionPerformed();
    void updateDisplay();
    void updateSelectedTime(QTime aTime);
    void updateSelectedDate(QDate aDate);
    void useCustomTimeActionPerformed();
    void updateMonthOrYear(int y, int m);
//    void closeApp();

signals:
//    void sendQuit();

};

#endif // ANIMATEDIALOG_H
