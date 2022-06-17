
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
    
    binfo("icon theme: {}", QIcon::themeName());
    for(auto i: QIcon::themeSearchPaths())
        binfo("icon paths: {}", i);

    Mainwindow w;
    DownloadManager::instance(&w);
    w.show();
    return app.exec();
}
