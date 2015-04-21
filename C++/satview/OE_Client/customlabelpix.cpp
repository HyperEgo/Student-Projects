#include "customlabelpix.h"

//customLabelPix::customLabelPix(QObject *parent) :
//    QLabel(parent)
//{
//}


CustomLabelPix::CustomLabelPix(QWidget *parent) :
    QLabel(parent)
{
    // derp?
}


CustomLabelPix::CustomLabelPix(const QPixmap& pix, Satellite& satz, double scale, QWidget* parent): QLabel(parent)
{
    // pass in reference to pixmap; set pixmap
    // pass in reference to satellite object >> bring to local variable >> echo local variable in mousePressEvent

    int h = pix.height();
    int w = pix.width();
    h = h*0.75;  // fudge factor to scale native pixmap image size
    w = w*0.75;  // fudge factor to scale native pixmap image size
    setStyleSheet("background-color: rgba(0,0,0,0%)");
    setPixmap(pix.scaled(w*scale, h*scale, Qt::IgnoreAspectRatio, Qt::FastTransformation));
//    setPixmap(pix);
    sat=satz;
}


void CustomLabelPix::mousePressEvent(QMouseEvent* event) {  // reimplement event handler

    if (event && event->button() == Qt::LeftButton) {  // get cursor position
        QPointF p = event->pos();
        emit updateString(sat.getSatInfo());
//        emit updateString("Congratulations! You can click stuff!");

        qDebug()<<"You clicked a satellite."
                <<"Satellite pos = "<<p;
    }

}

// testing repository save state!!
