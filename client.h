#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QMessageBox>
#include <QString>
#include <QTime>

namespace Ui {
class Client;
}

class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = 0);
    ~Client();
    enum sendIdentifier
    {
        message = 1,
        name = 2
    };
    void connectChat(const QString & hostname, qint16 port);
    void sendData(sendIdentifier sID, const QString & data);

public slots:
    void socketError(QAbstractSocket::SocketError socketerror);
    void disconnected();
    void readyRead();

private slots:
    void on_sendButton_clicked();
    void on_lineEdit_returnPressed();
    void on_lineEdit_textChanged(const QString &arg1);
    void changeEvent(QEvent * event);

private:
    void addToList(const QString & name);
    void removeFromList(const QString & name);
    void print(const QString & text, const QString & color = "black");
    void closeEvent(QCloseEvent *closeevent);
    QTcpSocket* tcpSocket;
    QList<QString> connectedNames;
    QString activeTitle;
    int unread{0};
    Ui::Client *ui;
};

#endif // CLIENT_H
