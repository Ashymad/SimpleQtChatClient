#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent) :
    QWidget(parent,Qt::Window),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    ui->sendButton->setEnabled(false);
    tcpSocket = new QTcpSocket;
    ui->lcdNumber->display(0);
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(socketError(QAbstractSocket::SocketError)));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(tcpSocket, SIGNAL(readyRead()),this,SLOT(readyRead()));
}

void Client::sendData(sendIdentifier sID, const QString &data){
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_4);
    out << (quint16)sID;
    out << data;
    tcpSocket->write(block);
}

void Client::print(const QString & text, const QString & color)
{
    ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->insertHtml("<font color=\""+color+"\">"
                                +text.toHtmlEscaped()+"</font><br>");
}

void Client::closeEvent(QCloseEvent *closeevent)
{
    disconnected();
}

void Client::on_lineEdit_returnPressed()
{
    if(ui->lineEdit->text().size() > 0)
    {
        on_sendButton_clicked();
    }
}

void Client::on_lineEdit_textChanged(const QString &arg1)
{
    ui->sendButton->setEnabled(arg1.size() > 0);
}

void Client::on_sendButton_clicked()
{
    QString message = ui->lineEdit->text();
    ui->lineEdit->clear();
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
            print("List of commands:","grey");
            print("\t/help","grey");
            print("\t/close","grey");
        }
        else
        {
            print("Unkown command: " + message,"grey");
            print("For list of commands write: /help","grey");
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
                                         "Make sure the chat server is running, "
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
    deleteLater();
}

void Client::connectChat(const QString & hostname, qint16 port)
{
    tcpSocket->abort();
    tcpSocket->connectToHost(hostname,port);
    tcpSocket->waitForConnected();
}

void Client::readyRead()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_4);
    qint16 identifier;
    QVector<QString> data;
    QTime time;
    in >> identifier;
    in >> data;
    in >> time;
    switch(identifier)
    {
    case 1: //ClientHandler::message
        print("["+time.toString("hh:mm:ss")+"] <"+data[1]+"> "+data[0]);
        break;
    case 2: //ClientHandler::joined
        print("["+time.toString("hh:mm:ss")+"] *"+data[0]+" has joined.","grey");
        addToList(data[0]);
        break;
    case 3: //ClientHandler::left
        print("["+time.toString("hh:mm:ss")+"] *"+data[0]+" has left.","grey");
        removeFromList(data[0]);
        break;
    case 4: //ClientHandler::list
        for(QString & qs : data)
        {
            addToList(qs);
        }
        break;
    default:
        qDebug() << "Error: server send data with an unkown identifier (" << identifier << ")";
        tcpSocket->readAll();
    }

    if(!isActiveWindow())
    {
        if (unread == 0)
        {
            activeTitle = windowTitle();
            setWindowTitle(activeTitle + " [1]");
            unread++;
        }
        else
        {
            unread++;
            setWindowTitle(activeTitle + " [" + QString::number(unread) + "]");
        }
    }
}


void Client::addToList(const QString & name)
{
    ui->textBrowser_2->moveCursor(QTextCursor::End);
    ui->textBrowser_2->append(name);
    connectedNames.append(name);
    ui->lcdNumber->display(
                ui->lcdNumber->intValue() + 1);
}

void Client::removeFromList(const QString & name)
{
    connectedNames.removeOne(name);
    ui->textBrowser_2->clear();
    for(QString & qs : connectedNames)
    {
        ui->textBrowser_2->append(qs);
    }
    ui->lcdNumber->display(
                ui->lcdNumber->intValue() - 1);
}
void Client::changeEvent(QEvent * event)
{
    if(event->type() == QEvent::ActivationChange)
    {
        if(unread > 0)
        {
            setWindowTitle(activeTitle);
        }
        unread = 0;

    }
}

Client::~Client()
{
    close();
    delete ui;
    tcpSocket->deleteLater();
}

