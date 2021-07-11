#ifndef TSERVER_H
#define TSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QtConcurrent>

class TServer : public QObject
{
    Q_OBJECT
public:
    explicit TServer(QObject *parent = nullptr);
    TServer(QString str);
    ~TServer();
    bool listening;

signals:

private slots:
    void newClient();

private:
    QTcpServer* server;
    QList<QTcpSocket*>* clients;
    QFuture<void> processClient();
    void workingPrimitive();
};

#endif // TSERVER_H
