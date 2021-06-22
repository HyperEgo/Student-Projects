#include "customgraphicspixmapitem.h"
#include <QtDebug>

//CustomGraphicsPixmapItem::CustomGraphicsPixmapItem(QGraphicsItem *parent) :
//    QGraphicsPixmapItem(parent)
//{


//}


//CustomGraphicsPixmapItem::CustomGraphicsPixmapItem()
//{

//}


CustomGraphicsPixmapItem::CustomGraphicsPixmapItem(QPixmap pixy) :
    QGraphicsPixmapItem(pixy)
{
    dx = 0.00;
    dy = 0.05;
    x = 0.0;
    y = 0.0;
}


void CustomGraphicsPixmapItem::advance(int step) {

    // do something
    QPointF f = this->scenePos();
    qDebug()<<"scene pos ="<<f;

    if (step == 0)
        return;
    x = x + dx;
//    y = y + dy;
    y = dy;
    setPos(mapToScene(x, y));

    if (f.y()>500) {
//        f.setY(0);
        QPointF z = this->scenePos();
        this->setPos(z.x(), 0);
    }

}
