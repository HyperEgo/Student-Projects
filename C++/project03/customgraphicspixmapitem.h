#ifndef CUSTOMGRAPHICSPIXMAPITEM_H
#define CUSTOMGRAPHICSPIXMAPITEM_H

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QPixmap>
#include <QObject>

//#include <QPainter>
//#include <QRect>

class CustomGraphicsPixmapItem : public QGraphicsPixmapItem
{
    //Q_OBJECT

public:
//    explicit CustomGraphicsPixmapItem(QGraphicsItem *parent = 0);
//    CustomGraphicsPixmapItem();

    CustomGraphicsPixmapItem(QPixmap pixy);
//    QRectF boundingRect() const;
//    QPainterPath shape() const;
//    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);



protected:
    void advance(int step);


private:
    qreal dx, dy;
    qreal x, y;
    qreal w, h;

signals:

public slots:

};

#endif // CUSTOMGRAPHICSPIXMAPITEM_H
