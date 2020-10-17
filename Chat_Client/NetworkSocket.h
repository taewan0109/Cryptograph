#ifndef NETWORKSOCKET_H
#define NETWORKSOCKET_H

#include <QTcpSocket>

class QTcpServer;
class QNetworkSession;
class Communicate
{
private:
    void initialize();
    QTcpSocket *tcpSocket;

private:
    void connectButton();
    void readMessage();
    void disconnected();
};

#endif // NETWORKSOCKET_H
