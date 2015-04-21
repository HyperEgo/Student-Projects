/*  Project CHOPPA: Alex Harmon, Jacob Walsh, Fleance Collins
 *  CPE 496: 1/19/15
 *
 *  source: http://stackoverflow.com/questions/20546750/qtcpsocket-reading-and-writing
 *  > added socket error checking notifications to the user
 *  > added minor implementation argument and calling details for working with client classes
*/

#include <QtCore>
#include <QtNetwork>
#include <QMessageBox>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);

    bool connectToHost(QString, int);

public slots:
    bool writeData(QByteArray);
    void displayError(QAbstractSocket::SocketError socketError);  // Catch connection errors

private:
    QTcpSocket *socket;
};
