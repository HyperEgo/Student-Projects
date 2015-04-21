#include "tableview.h"
#include "ui_tableview.h"

tableView::tableView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tableView) {
    ui->setupUi(this);
    ui->progressBar->setMinimum(0);
    // A default value for the progressbar
    ui->progressBar->setMaximum(100);
}

tableView::~tableView() {
    delete ui;
}

void tableView::setData(QList<satData> *data) {
    satList = data;
    ui->progressBar->setMaximum(satList->size());
}

void tableView::on_getTableButton_clicked() {

    // Set progress to 0
    ui->progressBar->setValue(0);

    //This is to prevent the table Satellite to be over written
    model.setQuery("CREATE TEMPORARY TABLE Sat LIKE Satellite");

    //This is where the parsed data will be inserted to be placed in the temporary table before the user
    // hits accept or reject
    QSqlQuery query;
    query.prepare("INSERT INTO Sat (NORAD_ID, SatName, LaunchYear, "
                  "LaunchNum, FullEpoch, EpochYear, EpochDay, EpochUTC, "
                  "DragTerm, Inclination, Ascention, Eccentricity, Perigee, "
                  "MeanAnomaly, MeanMotion) VALUES (:NORAD_ID, :SatName, :LaunchYear, "
                  ":LaunchNum, :FullEpoch, :EpochYear, :EpochDay, :EpochUTC, "
                  ":DragTerm, :Inclination, :Ascention, :Eccentricity, :Perigee, "
                  ":MeanAnomaly, :MeanMotion);");

    // Iterate through the QList
    int count = 0;
    QList<satData>::iterator i;
    for (i = satList->begin(); i != satList->end(); ++i) {
        query.bindValue(":NORAD_ID", i->getNoradID());
        query.bindValue(":SatName", i->getSatName());
        query.bindValue(":LaunchYear", i->getLaunchYear());
        query.bindValue(":LaunchNum", i->getLaunchNumber());
        query.bindValue(":FullEpoch", i->getFullEpoch());
        query.bindValue(":EpochYear", i->getEpochYear());
        query.bindValue(":EpochDay", i->getEpochDay());
        query.bindValue(":EpochUTC", i->getEpochTime());
        query.bindValue(":DragTerm", i->getDragCoefficient());
        query.bindValue(":Inclination", i->getOrbitInclination());
        query.bindValue(":Ascention", i->getRightAscentionAscendingNode());
        query.bindValue(":Eccentricity", i->getEccentricity());
        query.bindValue(":Perigee", i->getPerigee());
        query.bindValue(":MeanAnomaly", i->getMeanAnomaly());
        query.bindValue(":MeanMotion", i->getMeanMotion());
        query.exec();
        ui->progressBar->setValue(++count);
    }

    model2.setQuery("DELETE FROM Satellite2");
    model3.setQuery("DELETE FROM ClientTool.ClientSatellite2");
    model2.setQuery("INSERT INTO Satellite2 SELECT * FROM Satellite");
    model3.setQuery("INSERT INTO ClientTool.ClientSatellite2 SELECT * FROM Satellite");
    model.setQuery("SELECT * FROM Sat");
    tempTable.setModel(&model);
    tempTable.setWindowTitle("Table to Upload");
    // Can we center the temp table and maximize it to fit all rows?
    tempTable.show();

    ui->acceptButton->setEnabled(true);
    ui->rejectButton->setEnabled(true);
}

void tableView::on_acceptButton_clicked()
{
    model.setQuery("DELETE FROM Satellite");
    model3.setQuery("DELETE FROM ClientTool.ClientSatellite");
    model3.setQuery("INSERT INTO ClientTool.ClientSatellite SELECT * FROM Sat");
    model.setQuery("INSERT INTO Satellite SELECT * FROM Sat");
    model.setQuery("SELECT * FROM Satellite");
    ui->tableview->setModel(&model);
    tempTable.close();

    ui->okButton->setEnabled(true);
}

void tableView::on_rejectButton_clicked()
{
    model2.setQuery("SELECT * FROM Satellite2");
    ui->tableview->setModel(&model2);
    tempTable.close();

    ui->okButton->setEnabled(true);
}

void tableView::on_okButton_clicked()
{
    close();
    model.setQuery("DELETE FROM Sat");
}


