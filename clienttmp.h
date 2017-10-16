#ifndef CLIENT_H
#define CLIENT_H

#include <QThread>
#include <QTcpSocket>
#include <QWidget>
#include <QMessageBox>
#include <QString>
#include "clientwindow.h"

class Client : public QThread
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    ~Client();
    enum sendIdentifier
    {
        message = 1,
        name = 2
    };
    void connectChat(const QString & hostname, qint16 port);
    void createWindow(QWidget * parent = 0);
    void sendData(sendIdentifier sID, const QString & data);
signals:

public slots:
    void socketError(QAbstractSocket::SocketError socketerror);
    void disconnected();
    void readyRead();
    void sendButton(const QString & message);
private:
    QTcpSocket* tcpSocket;
    ClientWindow * clientWindow;
};

#endif // CLIENT_H
