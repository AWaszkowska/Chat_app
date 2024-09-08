#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    chatClient(nullptr)
{
    ui->setupUi(this);

    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::connectToServer);
    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::sendMessage);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete chatClient;
}

void MainWindow::connectToServer()
{
    qDebug() << "Connect button clicked";

    QString ip = ui->ip->text();
    QString port = ui->port->text();

    if(ip.isEmpty() || port.isEmpty())
    {
        ui->msg_history->append("Enter a valid IP address");
        return;
    }

    asio::ip::tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(ip.toStdString(), port.toStdString());

    chatClient = new client(io_context, endpoints);

    std::thread([this]() {io_context.run(); }).detach();

    ui->msg_history->append("Connected to " + ip + ": " + port);
}
void MainWindow::sendMessage()
{
    if(!chatClient)
    {
        QMessageBox::warning(this, "Error", "Not connected to any server");
        return;
    }

    QString message = ui->send_msg->text();
    if(message.isEmpty())
        return;
    chatClient->write(message.toStdString());
    ui->send_msg->clear();
}
void MainWindow::displayMessage(const QString& msg)
{
    ui->msg_history->append(msg);
}
