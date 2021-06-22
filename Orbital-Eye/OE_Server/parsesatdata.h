#ifndef PARSESATDATA_H
#define PARSESATDATA_H

#include <QString>
#include <QFile>
#include <QList>
#include <QDebug>
#include "satdata.h"



class parseSatData : public QObject
{
    Q_OBJECT

public:
    parseSatData();
    parseSatData(QFile *aFile);
    parseSatData(QString fileAndPath);
    ~parseSatData();

    QList<satData> satList;

signals:
    void finishedParsing();

private:
    QFile* theCatalog;
    QString* theCatalogFileAndPath;

public slots:
    void parseOne(void);
    void parseAll(void);
    QList<satData> returnResults();

};

#endif // PARSESATDATA_H
