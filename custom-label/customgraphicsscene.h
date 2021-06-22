#ifndef CUSTOMGRAPHICSSCENE_H
#define CUSTOMGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QWidget>
#include <QMimeData>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsView>

#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QTimer>

#include "customgraphicspixmapitem.h"

class CustomGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CustomGraphicsScene(QObject *parent = 0);
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);


private:
//    CustomGraphicsPixmapItem* cg;

signals:

public slots:

};

#endif // CUSTOMGRAPHICSSCENE_H
