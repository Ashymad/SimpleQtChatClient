#include "clienttmp.h"

Client::Client(QObject *parent) :
    QThread(parent)
{
    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(socketError(QAbstractSocket::SocketError)));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(tcpSocket, SIGNAL(readyRead()),this,SLOT(readyRead()));
}

void Client::createWindow(QWidget * parent)
{
    clientWindow = new ClientWindow(parent);
    connect(clientWindow,SIGNAL(sendButtonClicked(QString)),this,SLOT(sendButton(QString)));
    connect(clientWindow,SIGNAL(closed()),this,SLOT(disconnected()));
    clientWindow->show();
}

void Client::sendData(sendIdentifier sID, const QString &data){
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_4);
    out << (quint16)sID;
    out << data;
    tcpSocket->write(block);
}

void Client::sendButton(const QString &message)
{
    if(message[0] != '/')
    {
        sendData(Client::message, message);
    }
    else{
        if(message == "/close")
        {
            disconnected();
        }
        else if(message == "/help")
        {
            clientWindow->print("List of commands:\n"
                                "/help\n"
                                "/close");
        }
        else
        {
            clientWindow->print("Unkown command: " + message);
            clientWindow->print("For list of commands write: /help");
        }
    }
}

void Client::socketError(QAbstractSocket::SocketError socketerror)
{
    QMessageBox * msg = new QMessageBox;
    switch (socketerror) {
         case QAbstractSocket::RemoteHostClosedError:
             break;
         case QAbstractSocket::HostNotFoundError:
             QMessageBox::information(msg, tr("Error"),
                                      tr("The host was not found. Please check the "
                                         "host name and port settings."));
             break;
         case QAbstractSocket::ConnectionRefusedError:
             QMessageBox::information(msg, tr("Error"),
                                      tr("The connection was refused by the peer. "
                                         "Make sure the fortune server is running, "
                                         "and check that the host name and port "
                                         "settings are correct."));
             break;
         default:
             QMessageBox::information(msg, tr("Error"),
                                      tr("The following error occurred: %1.")
                                      .arg(tcpSocket->errorString()));
    }
    this->disconnected();
}

void Client::disconnected()
{
    tcpSocket->disconnectFromHost();
    clientWindow->close();
    exit(0);
}

void Client::connectChat(const QString & hostname, qint16 port)
{
    tcpSocket->abort();
    tcpSocket->connectToHost(hostname,port);
    clientWindow->setWindowTitle(hostname + QString(":%1").arg(port));
    tcpSocket->waitForConnected();
}

void Client::readyRead()
{
    QDataStream in(tcpSocket);
    QString data;
    in >> data;
    clientWindow->print(data);
}

Client::~Client()
{
    tcpSocket->deleteLater();
    clientWindow->deleteLater();
}

