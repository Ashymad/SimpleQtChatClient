#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton->setEnabled(false);
    this->setWindowTitle("Chat!");
    clientFactory = new ClientFactory(this);

    //lock window size
    setMaximumSize(geometry().size());
    setMinimumSize(geometry().size());

    //set settings file location
    settings = new QSettings(QStandardPaths::writableLocation(
                    QStandardPaths::AppDataLocation) + "/settings.ini",
                    QSettings::IniFormat);

    //connect lineEdits
    connect(ui->lineEdit,SIGNAL(textChanged(QString)),
            this,SLOT(anyLineEditChanged(QString)));
    connect(ui->lineEdit_2,SIGNAL(textChanged(QString)),
            this,SLOT(anyLineEditChanged(QString)));
    connect(ui->lineEdit_3,SIGNAL(textChanged(QString)),
            this,SLOT(anyLineEditChanged(QString)));
}

MainWindow::~MainWindow()
{
    clientFactory->deleteLater();
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    clientFactory->create(
                ui->lineEdit_3->text(), //nick
                ui->lineEdit->text(), //host
                ui->lineEdit_2->text().toInt()); //port
}

void MainWindow::on_actionLoad_triggered()
{
    ui->lineEdit->setText(
                settings->value("host","").toString());
    ui->lineEdit_2->setText(
                settings->value("port","").toString());
    ui->lineEdit_3->setText(
                settings->value("nick","").toString());
}

void MainWindow::on_actionSave_triggered()
{
    settings->setValue("host",
                      ui->lineEdit->text());
    settings->setValue("port",
                      ui->lineEdit_2->text());
    settings->setValue("nick",
                      ui->lineEdit_3->text());
}

void MainWindow::closeEvent(QCloseEvent *closeevent)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Quit Chat!", "Are you sure you want to quit?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
       exit(0);
    }
    else
    {
       closeevent->ignore();
    }
}

void MainWindow::anyLineEditChanged(const QString &arg1)
{
    ui->pushButton->setEnabled(ui->lineEdit->text().size() > 0
                               && ui->lineEdit_2->text().size() > 0
                               && ui->lineEdit_3->text().size() > 0);
}


