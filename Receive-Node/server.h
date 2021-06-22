/*  Project CHOPPA: Alex Harmon, Jacob Walsh, Fleance Collins
 *  CPE 496: 1/19/15
 *
 *  source: http://stackoverflow.com/questions/20546750/qtcpsocket-reading-and-writing
 *  > added socket error checking notifications to the user
 *  > added minor implementation argument and calling details for working with client classes
*/

#include <QtCore>
#include <QtNetwork>
#include <QtDebug>
#include <QMessageBox>


class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);

    void connectListen(QString, int);
    void displayError();

signals:
    void dataReceived(QByteArray);

private slots:
    void newConnection();
    void disconnected();
    void readyRead();

private:
    QTcpServer *server;
    QHash<QTcpSocket*, QByteArray*> buffers; //We need a buffer to store data until block has completely received
    QHash<QTcpSocket*, qint32*> sizes; //We need to store the size to verify if a block has received completely
};
