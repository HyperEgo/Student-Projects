#include "dialog.h"
#include "ui_dialog.h"

#include "parsesatdata.h"
#include <QFileDialog>
#include <QProgressDialog>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    // Add logo
    ui->logoLabel->setPixmap(QPixmap(":/images/orbital_eye_logo_02.png").scaled(ui->logoLabel->size()));

    // Disable buttons until they are allowed
    ui->getCatButton->setDisabled(true);
    ui->buildTableButton->setDisabled(true);
    // Set button control booleans to false
    st_usernameEntered = false;
    st_passwordEntered = false;
    sql_usernameEntered = false;
    sql_passwordEntered = false;
    // also make sure table has been created
    tableHasBeenCreated = false;

    // SIGNALS AND SLOTS

    // Make sure the user enters Space-Track name and password by using class variables
    connect(ui->stUser_lineEdit, SIGNAL(returnPressed()), this, SLOT(updateST_username()));
    connect(ui->stUser_lineEdit, SIGNAL(editingFinished()), this, SLOT(updateST_username()));
    connect(ui->stPass_lineEdit, SIGNAL(returnPressed()), this, SLOT(updateST_password()));
    connect(ui->stPass_lineEdit, SIGNAL(editingFinished()), this, SLOT(updateST_password()));

    // Make sure the user enters SQL name and password by using class variables
    connect(ui->sqlUser_lineEdit, SIGNAL(returnPressed()), this, SLOT(updateSQL_username()));
    connect(ui->sqlUser_lineEdit, SIGNAL(editingFinished()), this, SLOT(updateSQL_username()));
    connect(ui->sqlPass_lineEdit, SIGNAL(returnPressed()), this, SLOT(updateSQL_password()));
    connect(ui->sqlPass_lineEdit, SIGNAL(editingFinished()), this, SLOT(updateSQL_password()));

    // Button slots
    connect(ui->getCatButton, SIGNAL(clicked()), this, SLOT(getCatButtonPressed()));
    connect(ui->parseCatButton, SIGNAL(clicked()), this, SLOT(parseCatalogButtonPressed()));
    connect(ui->buildTableButton, SIGNAL(clicked()), this, SLOT(buildTableButtonPressed()));

    // Combobox slot
    connect(ui->numSatsComboBox, SIGNAL(activated(int)), this, SLOT(numSatsComboBoxChanged()));
    numEntries = 100;
    ui->sqlTableSizeField->setText("100 Satellites");

}



Dialog::~Dialog()
{
    delete ui;
}

void Dialog::updateST_username() {
    // Get the user input and load into the class variable
    st_username = ui->stUser_lineEdit->text();
    if (!st_username.isEmpty()) {
        // Enable button if applicable
        st_usernameEntered = true;
        // Check if button should be enabled
        enableCatButton();
    }
}

void Dialog::updateST_password() {
    // Get the user input and load into the class variable
    st_password = ui->stPass_lineEdit->text();
    if (!st_password.isEmpty()) {
        st_passwordEntered = true;
        // Check if button should be enabled
        enableCatButton();
    }
}

void Dialog::updateSQL_username() {
    // Get the user input and load into the class variable
    sql_username = ui->sqlUser_lineEdit->text();
    if (!sql_username.isEmpty()) {
        sql_usernameEntered = true;
        // Check if button should be enabled
        enableBuildTableButton();
    }
}

void Dialog::updateSQL_password() {
    // Get the user input and load into the class variable
    sql_password = ui->sqlPass_lineEdit->text();
    if (!sql_password.isEmpty()) {
        sql_passwordEntered = true;
        // Check if button should be enabled
        enableBuildTableButton();
    }
}

void Dialog::numSatsComboBoxChanged() {
    switch (ui->numSatsComboBox->currentIndex()) {
    case 0:
        numEntries = 100;
        ui->sqlTableSizeField->setText("100 Satellites");
        break;
    case 1:
        numEntries = 1000;
        ui->sqlTableSizeField->setText("1000 Satellites");
        break;
    case 2:
        numEntries = 10000;
        ui->sqlTableSizeField->setText("10000 Satellites");
        break;
    default:
        numEntries = -1;
        ui->sqlTableSizeField->setText("Unlimited Satellites");
    }
    // Check if button should be enabled
    enableCatButton();
    qDebug() << "num entries: " << numEntries;
}

void Dialog::enableCatButton() {
    if(st_usernameEntered && st_passwordEntered && numEntries > -2) {
        ui->getCatButton->setEnabled(true);
    }
}



void Dialog::enableBuildTableButton() {
    if(sql_usernameEntered && sql_passwordEntered && tableHasBeenCreated) {
        ui->buildTableButton->setEnabled(true);
    }
}

void Dialog::getCatButtonPressed() {

    // Check if old catalog exists
    QFile test1("./currentCat.txt");
    if (test1.exists() && test1.size() == 0) {
        // Bad catalog, delete it
        qDebug() << "Delete empty catalog";
        test1.remove();
    } else if (test1.exists() && test1.size() > 0) {
        test1.rename("./currentCat.txt", "./fallbackCat.txt");
    }

    // Prompt user for save file name
    QString catalog = QFileDialog::getSaveFileName(this, tr("Choose a save file for the catalog"), "./currentCat.txt", tr("Text files (*.txt)"));
    // Exit function if user cancels
    if (catalog == NULL) return;
    // Test if file is openable and writable
    QFile test(catalog);
    test.open(QIODevice::ReadWrite);
    if (!test.isOpen() || !test.isWritable()) {
        QMessageBox::Retry == msgBox.information(&dialog, "Invalid Target!","Choose another file or location.\n",QMessageBox::Retry);
        test.close();
        return;
    }

    // Instantiate the grabber and set the variables
    grabber = new NetCatGrabber(this);
    grabber->setCatFilePath(catalog);
    grabber->setUserName(st_username);
    grabber->setUserPass(st_password);
    grabber->setCatalogSize(numEntries);
    // Next operation takes time, set QProgressDialog to indeterminate
    progress = new QProgressDialog("Fetching catalog...", "Abort", 0, 0, this);
    // Make it modal, display, and connect to the finished signal
    progress->setWindowModality(Qt::WindowModal);
    progress->show();
    connect(grabber, SIGNAL(finishedDownload()), progress, SLOT(accept()));
    // Get the catalog
    grabber->getCatalog();
}

void Dialog::parseCatalogButtonPressed() {
    QString catalog = QFileDialog::getOpenFileName(this, tr("Load File"), "./", tr("Text files (*.txt)"));
    parser = new parseSatData(catalog);
    // Next operation takes time
    progress = new QProgressDialog("Parsing catalog...", "Abort", 0, 0, this);
    progress->setWindowModality(Qt::WindowModal);
    progress->show();
    connect(parser, SIGNAL(finishedParsing()), progress, SLOT(accept()));
    parser->parseAll();
    retList = new QList<satData>(parser->returnResults());
    int goodEntries = retList->size();
    qDebug() << "Size of data structure: " << goodEntries;
    if (goodEntries == 0) {
        // Failed, bad file?
        QMessageBox msgBox;
        msgBox.setWindowTitle("Parser Status");
        msgBox.setText("Parsing Failed!  Choose a valid file!");
        msgBox.exec();
    } else if (goodEntries < numEntries) {
        // Some satellites were not parsed
        int badEntries = numEntries - goodEntries;
        QString msg = "File had ";
        msg.append(QString::number(badEntries));
        msg.append(" bad entries!");
        QMessageBox msgBox;
        msgBox.setWindowTitle("Parser Status");
        msgBox.setText(msg);
        msgBox.exec();
        tableHasBeenCreated = true;
        // Check if button should be enabled
        enableBuildTableButton();
    } else {
        // Show success message
        QMessageBox msgBox;
        msgBox.setWindowTitle("Parser Status");
        msgBox.setText("Parsing Successful!");
        msgBox.exec();
        tableHasBeenCreated = true;
        // Check if button should be enabled
        enableBuildTableButton();
    }
}

void Dialog::buildTableButtonPressed(){
    qDebug() << "Build table button pressed";
    qDebug() << "Num rows for the table: " << retList->size();
    // Connect to database
    if (!connectToDatabase()) return;
}

bool Dialog::connectToDatabase() {
    db = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL"));//QSqlDatabase::addDatabase("QMYSQL");
    db->setHostName("pavelow.eng.uah.edu");
    db->setPort(3301);
    db->setUserName(sql_username);
    db->setPassword(sql_password);
    db->setDatabaseName("ServerTool");

    if(!db->open())
    {
        // Two cases here - bad credentials, or DB unavailable
        QString msg = db->lastError().text();
        qDebug() << msg;
        qDebug() << "Error unable to connect due to above error";
        if (msg.contains("Unknown MySQL server host")) {
            QMessageBox::Retry == msgBox.information(&dialog, "Database Unavailable.","Please retry later.",QMessageBox::Retry);
            return false;
        } else if (msg.contains("Access denied")) {
            QMessageBox::Retry == msgBox.information(&dialog, "Incorrect User Info","Either username or password is incorrect.\nPlease re-enter username and password.",QMessageBox::Retry);
            return false;
        } else {
            QMessageBox::Retry == msgBox.information(&dialog, "Unknown MySQL failure:",msg,QMessageBox::Retry);
            return false;
        }


    } else {
        qDebug() << "connected";
    }

    viewer = new tableView();
    viewer->setData(retList);
    viewer->show();

    return true;
}


