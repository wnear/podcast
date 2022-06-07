
#include "mainwindow.h"
#include <QApplication>


int main (int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("podcast");
    app.setApplicationDisplayName("podcast");
    app.setQuitOnLastWindowClosed(true);

    Mainwindow w;
    w.show();
    return app.exec();
}
