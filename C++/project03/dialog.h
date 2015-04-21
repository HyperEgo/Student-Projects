#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLabel>
//#include <QGraphicsView>
#include <QGraphicsScene>
#include <QtGui>
#include <QtSql>
#include <QtDebug>
#include <QGraphicsSceneMouseEvent>
#include "customlabel.h"
#include "customgraphicsscene.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    void setArg(QString argv);
    QString getArg();

private:
    Ui::Dialog *ui;

    QSqlDatabase db;
    QString str;

//    QGraphicsView* graphicsView;
    CustomGraphicsScene* gscene;

};

#endif // DIALOG_H
