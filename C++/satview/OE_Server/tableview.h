#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QDialog>
#include <QtSql>
#include <QString>
#include <QTableView>
#include <QFileDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QProgressDialog>
#include <QList>
#include "satdata.h"


namespace Ui {
class tableView;
}

class tableView : public QDialog
{
    Q_OBJECT

public:
    explicit tableView(QWidget *parent = 0);
    ~tableView();
    void setData(QList<satData> *data);


private slots:
    void on_getTableButton_clicked();

    void on_okButton_clicked();

    void on_acceptButton_clicked();

    void on_rejectButton_clicked();

private:
    Ui::tableView *ui;
    QSqlQueryModel model;
    QSqlQueryModel model2;
    QSqlQueryModel model3;
    QTableView tempTable;
    bool accept_reject_notclicked;
    QList<satData> *satList;
    QProgressDialog *progress;



};

#endif // TABLEVIEW_H
