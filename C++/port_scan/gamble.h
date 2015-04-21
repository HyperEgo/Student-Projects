#ifndef GAMBLE_H
#define GAMBLE_H

#include <QDialog>
#include <QIcon>
#include <QDebug>
#include <QMessageBox>
#include <QAction>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QList>
#include <QDir>
#include <QKeyEvent>

const int MODULUS=8;

namespace Ui {
class Gamble;
}

class Gamble : public QDialog
{
    Q_OBJECT

public:
    explicit Gamble(QWidget *parent = 0);
    ~Gamble();

    QPixmap* getPicture();
    void setFlag (bool);
    bool getFlag();
    void loadPics();
    bool eventFilter(QObject *watched, QEvent *e);

private:
    Ui::Gamble *ui;
    bool flag;
    QList<QPixmap*> *luckyPics;
    QList<QPixmap*> *bunnyPics;


private slots:
    void infoMessage();
    void processSpin();
    void resetGamble();


signals:
    void closeApp();

};

#endif // GAMBLE_H
