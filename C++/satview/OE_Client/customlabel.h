#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H

#include <QLabel>
#include <QtDebug>
#include <QMouseEvent>
#include <QEvent>
#include <QString>

class customLabel : public QLabel
{
    Q_OBJECT
public:
//    explicit customLabel(QObject *parent = 0);
    customLabel(QObject *parent = 0);
    void mousePressEvent(QMouseEvent* event);

private:
    QPointF p;

signals:
    void updateCursor(QPointF);

public slots:

};

#endif // CUSTOMLABEL_H
