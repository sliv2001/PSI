#include "tserver.h"

TServer::TServer(QObject *parent)
{
    this->server = new QTcpServer();
    bool done= 0;
    for(int i=55555; i<55565; i++)
        if (server->listen(QHostAddress::Any, i)){
            done=1;
            break;
        }
    listening=done;
    clients = new QList<QTcpSocket*>();

    connect(server, &QTcpServer::newConnection, this, &TServer::newClient);
}

TServer::TServer(QString str)
{
    this->server = new QTcpServer();
    bool done= 0;
    QHostAddress addr(str);
    for(int i=55555; i<55565; i++)
        if (server->listen(addr, i)){
            done=1;
            break;
        }
    listening=done;
    clients = new QList<QTcpSocket*>();
}

TServer::~TServer()
{
    delete this->server;
    delete this->clients;
}

void TServer::newClient()
{
    clients->append(server->nextPendingConnection());
    processClient();
}

QFuture<void> TServer::processClient()
{
    return QtConcurrent::run(&TServer::workingPrimitive, this);

}

void TServer::workingPrimitive()
{
    int i = clients->count();
}
