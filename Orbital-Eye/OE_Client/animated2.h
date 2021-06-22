#ifndef ANIMATED2_H
#define ANIMATED2_H

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
#include <QDir>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGraphicsProxyWidget>
#include <QGraphicsItemGroup>

#include "satdata.h"
#include "satellitemanager.h"
#include "customlabelpix.h"
#include "customgraphicsscene.h"

const int MAX = 5;


namespace Ui {
class AnimateD2;
}

class AnimateD2 : public QDialog
{
    Q_OBJECT

public:
    explicit AnimateD2(QWidget *parent = 0);
    ~AnimateD2();
    void setAllParameters(QList<satData> *aList, double lat, double lon, double boreAZ, double boreEL, double fovDegrees, int radiusInPixels);

    void loadSats();

private:
    Ui::AnimateD2 *ui;
    QWidget *theParent;
    CustomGraphicsScene *myScene;
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
    qint64 animationTime;
    QTime userTime;
    QDate userDate;
    bool timeInitialized;

    // JWJ optional mods
    int multiplier;
    int minimumRange;
    int maximumRange;
    void checkRangeBounds(bool min, int value);
    void updateTimestring(qint64 time);
    QList<int> satsViewed;
    void updateSatsViewed(int ID);
    QGraphicsItemGroup *group;
    bool groupRemoved;
    bool showTrails;

    QList<QPixmap*> *satPics;
    CustomLabelPix *cus;
    QPixmap *bg;


private slots:
    void startButtonActionPerformed();
    void updateDisplay();
    void updateSelectedTime(QTime aTime);
    void updateSelectedDate(QDate aDate);
    void useCustomTimeActionPerformed();
    void updateMonthOrYear(int y, int m);

    // JWJ optional mods
    void clearButtonActionPerformed();
    void maxRngSliderActionPerformed(int maxRng);
    void minRngSliderActionPerformed(int minRng);
    void timeMultiplierActionPerformed(int multi);
    void showTrailsCheckboxActionPerformed();


    void getSatString(QString);
    void stopTime();

signals:

};

#endif // ANIMATED2_H
