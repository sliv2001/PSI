#include <QBuffer>
#include "mainwindow.h"
#include "tclient.h"

TClient::TClient(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()), this, SLOT(on_socketConnected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(on_socketDisconnected()));
    connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(on_socketDisplayError(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(readyRead()), this, SLOT(on_socketReadyRead()));
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
    if (connected){
        QDataStream out(socket);
        out.setVersion(QDataStream::Qt_6_2);
        out<<file.pictureCode;
        QPixmap pixmap(file.fullPath);
        QByteArray bytes;
        QBuffer buffer(&bytes);
        buffer.open(QIODevice::WriteOnly);
        pixmap.save(&buffer, "JPG");
        out<<bytes;
        socket->flush();
    }
}

QVector<QByteArray> TClient::parseResponse()
{
    QDataStream in(socket);
    QVector<QByteArray> res;
    QByteArray instance, amount;
    int a;

    in.startTransaction();
    in>>instance;
    res.append(instance);
    in>>amount;

    if ((a=amount[0])<0){
        res.clear();
        qWarning("Server returned wrong amount of objects: %d", a);
        return res;
    }
    for (int i=0; i<a*3; i++) /* x3 because for each object 3 pieces of data */
    {
        in>>instance;
        res.append(instance);
    }
    if (!in.commitTransaction())
        res.clear();
    return res;
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

void TClient::on_socketReadyRead()
{
    QVector<QByteArray> result = parseResponse();
    if (result.isEmpty())
        return;
    ((TContext*)(this->parent()))->updateModelWithResult(result);
}
