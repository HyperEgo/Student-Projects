#include "customgraphicsscene.h"
#include <QtDebug>

CustomGraphicsScene::CustomGraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
    //derp?

}


void CustomGraphicsScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event) {

    if (event && event->mimeData()) {
        const QMimeData* md = event->mimeData();

        if (md->hasImage()) {
            event->acceptProposedAction();
        }
    }

}


void CustomGraphicsScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event) {

    if (event && event->mimeData()) {
        const QMimeData* md = event->mimeData();

        if (md->hasImage()) {
            event->acceptProposedAction();
        }
    }

    qDebug()<<"drag move event executed";

}


void CustomGraphicsScene::dropEvent(QGraphicsSceneDragDropEvent *event) {

    QPixmap pic;
    if(event && event->mimeData()) {
        const QMimeData* md = event->mimeData();

        if(md->hasImage()) {
            pic = md->imageData().value<QPixmap>();  //retrieve image as pixmap
        }

    }
    if (!pic.isNull()) {

        qDebug()<<"Drop Event Executed.\n";

        CustomGraphicsPixmapItem* cp = new CustomGraphicsPixmapItem(pic);

        QPointF p = event->scenePos();  //get cursor position locally
        qDebug()<<"cursor pos ="<<p;

        cp->setPos(p);
        this->addItem(cp);

        QTimer* timer = new QTimer;
        connect(timer, SIGNAL(timeout()), this, SLOT(advance()));
        timer->setInterval(1000/33);
        timer->start();

//        QTimeLine* timer = new QTimeLine(5000);
//        timer->setFrameRange(0, 100);

//        QGraphicsItemAnimation* anime = new QGraphicsItemAnimation;
//        anime->setItem(cp);
//        anime->setTimeLine(timer);

//        int pz = static_cast<int>(p.manhattanLength());
//        for(int i = 0; i < 200; ++i) { anime->setPosAt(i / 200.0, QPointF(0, i));}
//        for(int i = pz; i <= 500; ++i) {
//            anime->setPosAt(i / 500.0, QPointF(0, i));
//            QPointF fp = anime->posAt(1);
//            qDebug()<<"set pos = "<<fp;
//        }

//        timer->start();
    }

}
