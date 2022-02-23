#include <QBuffer>
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
        socket->write((char*)&size, 8);                     //Send length of file

        auto data = f.readAll();
        socket->write(data);                                //Send file
    }
    f.close();
}

void TClient::sendFile(TMediaFile file)
{
    /*Проверка на подключенность сокета проведена в функции выше по стеку*/
    QDataStream out(socket);
    out.setVersion(QDataStream::Qt_6_2);
    out<<file.pictureCode;
    QPixmap pixmap(file.fullPath);
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "JPG");
    out<<bytes;
}

void TClient::on_socketConnected()
{
    qInfo("Socket Connected");
    connected=1;
}

void TClient::on_socketDisconnected()
{
    qInfo("Socket Disconnected");
    connected=0;
}

void TClient::on_socketDisplayError(QAbstractSocket::SocketError)
{
    qWarning("%s", socket->errorString().toStdString().c_str());
    connected=0;
}
