#include <QApplication>
#include <QDebug>
#include <asio.hpp>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec(); // Start the Qt event loop
}
