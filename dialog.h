#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include <QMessageBox>
#include <QKeyEvent>
#include <QTcpSocket>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void keyPressEvent(QKeyEvent *event);

private slots:
    void handleControl();
    void connectedToSocket();

private:
    Ui::Dialog *ui;
    QTcpSocket *socket;
    bool isConnected;
};

#endif // DIALOG_H
