/*#include "NetworkSocket.h"
#include <QHostAddress>
#include <QDebug>
#include <QTcpSocket>

void Communicate::initialize()
{
    ;
    tcpSocket = new QTcpSocket(this);
}

void Communicate::connectButton()
{
    QString serverip   = "172.18.48.161";   //ui->serverIP->text().trimmed();

    QHostAddress serverAddress(serverip);
    tcpSocket->connectToHost(serverAddress, 25000);
}

void Communicate::readMessage()
{
    if(tcpSocket->bytesAvailable() >= 0)
    {
        QByteArray readData = tcpSocket->readAll();
        //ui->textEdit->append(readData);
    }
}

void Communicate::disconnected()
{
    qDebug() << Q_FUNC_INFO << "서버 접속 종료.";
}
*/
