#ifndef TCLIENT_H
#define TCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QFile>
#include "tmediafile.h"

class TClient : public QObject
{
    Q_OBJECT
public:
    explicit TClient(QObject *parent = nullptr);
    ~TClient();

    /**
     * @brief sendFile sends whole file to server
     * @param path
     */
    void sendFile(QString path);

    /**
     * @brief sendFile sends media (PixMap, etc) to server
     * @param file
     */
    void sendFile(TMediaFile file);

    bool connected=0;

signals:

private slots:
    void on_socketConnected();
    void on_socketDisconnected();
    void on_socketDisplayError(QAbstractSocket::SocketError);

private:
    QTcpSocket* socket;
};

#endif // TCLIENT_H
