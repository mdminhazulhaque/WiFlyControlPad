#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    isConnected = false;

    ui->setupUi(this);
    setWindowTitle("WiFlyControlPad");

    connect(ui->bExit, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->bClear, SIGNAL(clicked()), ui->datastream, SLOT(clear()));
    connect(ui->bControl, SIGNAL(clicked()), this, SLOT(handleControl()));

    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()), this, SLOT(connectedToSocket()));
}

Dialog::~Dialog()
{
    socket->disconnectFromHost();
    delete ui;
}

void Dialog::keyPressEvent(QKeyEvent *event)
{
    if(isConnected)
    {
        QTextCursor *cursor = new QTextCursor(ui->datastream->textCursor());
        cursor->insertText(QChar(event->key() & 0xFF));
        char data[8]; // may require more or less, not sure
        data[0] = (event->key() & 0xFF);
        socket->write(data);
    }
}


void Dialog::handleControl()
{
    if(isConnected)
    {
        isConnected = false;
        ui->host->setReadOnly(false);
        ui->port->setReadOnly(false);
        socket->disconnectFromHost();
        ui->bControl->setText("Connect");
        return;
    }

    socket->disconnectFromHost();
    socket->connectToHost(ui->host->text(), ui->port->value());
    if(!socket->waitForConnected(1000))
        QMessageBox::critical(this,
                              "Error",
                              "Failed to connect to host");
}

void Dialog::connectedToSocket()
{
    isConnected = true;

    QMessageBox::information(this,
                             "Success",
                             "Successfully connected to host");
    ui->host->setReadOnly(true);
    ui->port->setReadOnly(true);
    ui->bControl->setText("Disconnect");
    ui->datastream->setFocus();
}
