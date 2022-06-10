
#include "mainwindow.h"
#include <QApplication>

#include "downloadmanager.h"
int main (int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("podcast");
    app.setApplicationDisplayName("podcast");
    app.setQuitOnLastWindowClosed(true);

    Mainwindow w;
    DownloadManager::instance(&w);
    w.show();
    return app.exec();
}
