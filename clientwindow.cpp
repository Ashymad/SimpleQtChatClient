#include "clientwindow.h"
#include "ui_clientwindow.h"

ClientWindow::ClientWindow(QWidget *parent) :
    QWidget(parent,Qt::Window),
    ui(new Ui::ClientWindow)
{
    ui->setupUi(this);
    ui->sendButton->setEnabled(false);
}

ClientWindow::~ClientWindow()
{
    delete ui;
}

void ClientWindow::print(const QString & text)
{
    ui->textBrowser->append(text);
}

void ClientWindow::on_sendButton_clicked()
{
    emit sendButtonClicked(ui->lineEdit->text());
    ui->lineEdit->clear();
}

void ClientWindow::closeEvent(QCloseEvent *closeevent)
{
    emit closed();
}

void ClientWindow::on_lineEdit_returnPressed()
{
    if(ui->lineEdit->text().size() > 0)
    {
        on_sendButton_clicked();
    }
}

void ClientWindow::on_lineEdit_textChanged(const QString &arg1)
{
    ui->sendButton->setEnabled(ui->lineEdit->text().size() > 0);
}
