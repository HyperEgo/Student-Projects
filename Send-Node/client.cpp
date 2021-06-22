#include "client.h"
#include <QtDebug>

static inline QByteArray IntToArray(qint32 source);

Client::Client(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    // QMessageBox cannot work directly with a class not derived from QWidget therefore QDialog is needed
    QDialog d;

    switch (socketError)
    {
        case QAbstractSocket::RemoteHostClosedError:
            QMessageBox::information(&d, tr("IPShare Client"),
                                    tr("The remote host is closed. Please check the "
                                    "host name and make sure it is sharing."));
            qDebug()<<"IPShare Client: Remote Host Closed Error.";
            break;
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::information(&d, tr("IPShare Client"),
                                     tr("The host is not found. Please check the "
                                        "host name and make sure it is sharing."));
            qDebug()<<"IPShare Client: Host Not Found.  Please check the host name and make sure it is sharing.";
            break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information(&d, tr("IPShare Client"),
                                     tr("The connection is refused by the peer. "
                                        "Make sure the other side is sharing, "
                                        "and check that the host name and port "
                                        "settings are correct."));
            qDebug()<<"IPShare Client: Connection Refused.  Make sure the other side is sharing, check hostname and port.";
            break;
        default:
            QMessageBox::information(&d, tr("IPShare Client"),
                                     tr("The following error occurred: ")
                                     .arg(socket->errorString()));
            qDebug()<<"IPShare Client: the following error occurred, "+socket->errorString();
     }
    delete socket;
}

bool Client::connectToHost(QString host, int port)
{
    socket->connectToHost(host, port);
    return socket->waitForConnected();
}

bool Client::writeData(QByteArray data)
{
    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->write(IntToArray(data.size())); //write size of data
        socket->write(data); //write the data itself
        return socket->waitForBytesWritten();
    }
    else
        return false;
}

QByteArray IntToArray(qint32 source) //Use qint32 to ensure that the number have 4 bytes
{
    //Avoid use of cast, this is the Qt way to serialize objects
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}
