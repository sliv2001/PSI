#ifndef TCLIENT_H
#define TCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QFile>

class TClient : public QObject
{
    Q_OBJECT
public:
    explicit TClient(QObject *parent = nullptr);
    ~TClient();
    void sendFile(QString path);

signals:

private slots:
    void on_socketConnected();
    void on_socketDisconnected();
    void on_socketDisplayError(QAbstractSocket::SocketError);

private:
    QTcpSocket* socket;
};

#endif // TCLIENT_H
