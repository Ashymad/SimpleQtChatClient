#ifndef CLIENTFACTORY_H
#define CLIENTFACTORY_H

#include "client.h"
#include <QObject>

class ClientFactory : public QObject
{
    Q_OBJECT
public:
    explicit ClientFactory(QWidget * parent = 0);
    void create(const QString & name, const QString & hostname, qint16 port);
    ~ClientFactory();
private:
    QList<Client*> clients;
    QWidget * _parent;
};

#endif // CLIENTFACTORY_H
