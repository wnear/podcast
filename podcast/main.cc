
#include "mainwindow.h"
#include "log.h"
#include "global.h"
#include "sqlmanager.h"
#include <QApplication>

#include "downloadmanager.h"
int main(int argc, char *argv[]) {
    // TODO: Qt version check, if <6, then set this.
    // QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    // QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
    QApplication app(argc, argv);

    // app.setOrganizationName("nobody");
    app.setApplicationName("podcast");
    app.setApplicationDisplayName("podcast");
    app.setQuitOnLastWindowClosed(true);

    Log::init();

    Data::init();

    SQLManager::instance()->init();

    binfo("icon theme: {}", QIcon::themeName());
    QIcon::setThemeName("breeze");
    for (auto i : QIcon::themeSearchPaths()) binfo("icon paths: {}", i);

    Mainwindow w;
    DownloadManager::instance(&w);
    w.show();
    return app.exec();
}
