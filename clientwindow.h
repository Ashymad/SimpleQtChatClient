#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QWidget>

namespace Ui {
class ClientWindow;
}

class ClientWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ClientWindow(QWidget *parent = 0);
    void print(const QString & text);
    ~ClientWindow();

private slots:
    void on_sendButton_clicked();
    void on_lineEdit_returnPressed();

    void on_lineEdit_textChanged(const QString &arg1);

signals:
    void sendButtonClicked(const QString & text);
    void closed();
private:
    void closeEvent(QCloseEvent *closeevent);
    Ui::ClientWindow *ui;
};

#endif // CLIENTWINDOW_H
