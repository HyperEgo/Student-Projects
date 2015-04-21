#include "customgraphicsscene.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

CustomGraphicsScene::CustomGraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
}

void CustomGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
        QGraphicsScene::mousePressEvent(event); // this forwards the event to the item
        if (!event->isAccepted() && event->button() == Qt::LeftButton) {
             emit updateString("No satellite here.\n");
        }
}
