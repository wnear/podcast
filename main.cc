
#include "mainwindow.h"
#include "log.h"
#include "global.h"
#include <QApplication>

#include "downloadmanager.h"
int main (int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("podcast");
    app.setApplicationDisplayName("podcast");
    app.setQuitOnLastWindowClosed(true);

    Log::init();
    Data::init();

    Mainwindow w;
    DownloadManager::instance(&w);
    w.show();
    return app.exec();
}
