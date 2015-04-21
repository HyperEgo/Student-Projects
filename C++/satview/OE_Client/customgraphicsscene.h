#ifndef CUSTOMGRAPHICSSCENE_H
#define CUSTOMGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QString>

class CustomGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CustomGraphicsScene(QObject *parent = 0);

signals:
    void updateString(QString);

protected slots:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

};

#endif // CUSTOMGRAPHICSSCENE_H
