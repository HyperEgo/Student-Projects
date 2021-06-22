#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H

#include <QWidget>
#include <QLabel>
#include <QColor>
#include <QPainter>
#include <QPixmap>
#include <QPen>
#include <QBrush>

class CustomLabel : public QLabel
{
    Q_OBJECT
public:
//    explicit CustomLabel(QObject *parent = 0);
    CustomLabel(const QColor& color, QWidget* parent=0);
    void mousePressEvent(QMouseEvent* event);

private:
    QPixmap pixer;


signals:

public slots:

};

#endif // CUSTOMLABEL_H
