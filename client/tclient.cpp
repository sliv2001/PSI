#include "tclient.h"

TClient::TClient(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()), this, SLOT(on_socketConnected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(on_socketDisconnected()));
    connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(on_socketDisplayError(QAbstractSocket::SocketError)));
    socket->connectToHost("127.0.0.1", 55555);
}

TClient::~TClient()
{
    delete socket;
}

void TClient::sendFile(QString path)
{
    QFile f = QFile(path);
    if (f.open(QIODevice::ReadOnly)){
        uint64_t size = f.size();
        int32_t pathSize = path.length();
        socket->write((char*)&pathSize, 4);                 //Send length of path
        socket->write(path.toLocal8Bit(), path.length());   //Send path
        socket->write((char*)&size, 8);                     //Send length of file

        auto data = f.readAll();
        socket->write(data);                                //Send file
    }
    f.close();
}

void TClient::on_socketConnected()
{
    qInfo("done");
}

void TClient::on_socketDisconnected()
{
    qInfo("undone");
}

void TClient::on_socketDisplayError(QAbstractSocket::SocketError)
{
    qInfo("undone");
}
