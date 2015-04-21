#include "customlabel.h"

//customLabel::customLabel(QObject *parent) :
//    QLabel(parent)
//{
//}


customLabel::customLabel(QObject *parent)
{
    // derp?
}


void customLabel::mousePressEvent(QMouseEvent* event) {  // reimplement event handler

    if (event && event->button() == Qt::LeftButton) {  // get cursor position
        p = event->pos();
        emit updateCursor(p);
    }

//    qDebug()<<"Map cursor pos = "<<p;
}

