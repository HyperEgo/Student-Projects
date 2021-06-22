#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtSql>
#include <QDebug>
#include <QString>
#include <vector>
#include <QMessageBox>
#include <QProgressDialog>

#include "netcatgrabber.h"
#include "parsesatdata.h"
#include "satdata.h"
#include "tableview.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void parseCatalog();


private slots:
    // Slots for username and password entry, both ST and SQL
    void updateST_username();
    void updateST_password();
    void updateSQL_username();
    void updateSQL_password();

    // Slots for the buttons
    void getCatButtonPressed();
    void parseCatalogButtonPressed();
    void buildTableButtonPressed();

    // Slot for the combobox
    void numSatsComboBoxChanged();




private:
    Ui::Dialog *ui;
    QString st_username;
    QString st_password;
    QString sql_username;
    QString sql_password;
    NetCatGrabber *grabber;
    parseSatData *parser;
    int numEntries;
    // Parsed satellite data objects for SQL table
    QList<satData> *retList;

    // SQL database
    QSqlDatabase *db;
    tableView *viewer;
    // private function to connect to database
    bool connectToDatabase();

    // some booleans to ensure user data has been entered
    bool st_usernameEntered;
    bool st_passwordEntered;
    bool sql_usernameEntered;
    bool sql_passwordEntered;

    // boolean to esnsure table is ready to be uploaded
    bool tableHasBeenCreated;

    // private functions to enable buttons if user data entered
    void enableCatButton();
    void enableBuildTableButton();

    QMessageBox msgBox;
    QDialog dialog;
    QSqlQueryModel model;

    QProgressDialog *progress;


};

#endif // DIALOG_H
