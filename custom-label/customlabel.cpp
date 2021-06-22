#include "customlabel.h"
#include <QPixmap>
#include <QMouseEvent>
#include <QImage>
#include <QMimeData>
#include <QDrag>
#include <QRect>

//CustomLabel::CustomLabel(QObject *parent) :
//    QLabel(parent)
//{

//}


CustomLabel::CustomLabel(const QColor& color, QWidget* parent): QLabel(parent)
{
    //this->setFixedSize(50, 50);  //set label size
    QPixmap myMap(100, 70);
    myMap.fill(Qt::black);
    QPainter p(&myMap);
    QRect r(10, 10, 80, 50);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(QPen(color, 2, Qt::SolidLine, Qt::SquareCap));
    p.setBrush(QBrush(color, Qt::SolidPattern));
//    p.drawEllipse(10, 10, 40, 40);
    p.drawEllipse(r);

    setPixmap(myMap);  //make image from input color
    //myMap.save("timage.png", 0, -1);
    pixer=myMap;

}


void CustomLabel::mousePressEvent(QMouseEvent* event) {  //reimplement event handler

    if (event && event->button() == Qt::LeftButton) {

        //allocate and initialize mimedata object
        QMimeData* md = new QMimeData;
//        QImage pic(filename);
        //md->setImageData(QImage("timage.png"));
        md->setImageData(pixer);

        QDrag* drag = new QDrag(this);
        drag->setMimeData(md);

        if (pixmap()) {
            QSize s = this->sizeHint();
            drag->setPixmap(pixmap()->scaled(s));
        }
        drag->start();  //initiate drag
    }

}
