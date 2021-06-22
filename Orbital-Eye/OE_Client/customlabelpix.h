#ifndef CUSTOMLABELPIX_H
#define CUSTOMLABELPIX_H

#include <QLabel>
#include <QtDebug>
#include <QMouseEvent>
#include <QEvent>
#include <QString>
#include <QPixmap>

#include "satellite.h"

class CustomLabelPix : public QLabel
{
    Q_OBJECT

public:
//    explicit customLabelPix(QObject *parent = 0);

    CustomLabelPix(QWidget* parent = 0);
    CustomLabelPix(const QPixmap& pix, Satellite& satz, double scale, QWidget* parent=0);

    void mousePressEvent(QMouseEvent* event);

private:
    Satellite sat;


signals:
    void updateString(QString);


public slots:

};

#endif // CUSTOMLABELPIX_H

// testing repository save state!!
