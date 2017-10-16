#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QSettings>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QMessageBox>
#include <QStandardPaths>
#include "clientfactory.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_pushButton_clicked();
    void on_actionLoad_triggered();
    void closeEvent(QCloseEvent *closeevent);
    void on_actionSave_triggered();
    void anyLineEditChanged(const QString &arg1);
private:
    Ui::MainWindow *ui;
    QSettings * settings;
    ClientFactory * clientFactory;
};

#endif // MAINWINDOW_H
