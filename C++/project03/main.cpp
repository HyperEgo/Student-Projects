#include "dialog.h"
#include <QApplication>
#include <QtDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (QCoreApplication::arguments().at(1)=="") {
        qDebug()<<"No color table name provided.";
    }

    Dialog w;
    w.setArg(QCoreApplication::arguments().at(1));
    w.show();

    return a.exec();
}
