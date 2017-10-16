#include "clientfactory.h"

ClientFactory::ClientFactory(QWidget * parent) :
    QObject(parent)
{
}

void ClientFactory::create(const QString & name, const QString &hostname, qint16 port)
{
    Client * client = new Client();
    client->connectChat(hostname,port);
    client->sendData(Client::name,name);
    client->show();
    client->setWindowTitle(name+"@"+hostname + QString(":%1").arg(port));
    clients.push_back(client);
}

ClientFactory::~ClientFactory()
{
    for(Client * ptr : clients)
    {
        ptr->close();
        ptr->deleteLater();
    }
}

