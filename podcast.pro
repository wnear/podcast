######################################################################
# Automatically generated by qmake (3.1) Wed Jun 15 14:15:35 2022
######################################################################

include(header.pri)

TEMPLATE = app
TARGET = podcast
INCLUDEPATH += .

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += downloadmanager.h \
           downloadmanagerwgt.h \
           episode_detail_wgt.h \
           episode_treemodel.h \
           episode_treeview.h \
           episode_treewidget.h \
           episodedata.h \
           episodelistwgt.h \
           episodemodel.h \
           episodewgt.h \
           global.h \
           jsonop.h \
           localsave.h \
           log.h \
           mainwindow.h \
           network.h \
           notification.h \
           opmlparser.h \
           player.h \
           player_engine.h \
           podcast.h \
           poddata.h \
           podmodel.h \
           rssparser.h \
           setting.h \
           subnetwork.h \
           subpodcast.h \
           subsong.h \
           subsonic.h \
           utils.h
SOURCES += downloadmanager.cc \
           downloadmanagerwgt.cc \
           episode_detail_wgt.cc \
           episode_treemodel.cc \
           episode_treeview.cc \
           episode_treewidget.cc \
           episodedata.cc \
           episodelistwgt.cc \
           episodemodel.cc \
           episodewgt.cc \
           global.cc \
           jsonop.cc \
           localsave.cc \
           log.cc \
           main.cc \
           mainwindow.cc \
           network.cc \
           notification.cc \
           opmlparser.cc \
           player.cc \
           player_engine.cc \
           podcast.cc \
           poddata.cc \
           podmodel.cc \
           rssparser.cc \
           setting.cc \
           subnetwork.cc \
           subpodcast.cc \
           subsong.cc \
           subsonic.cc \
           utils.cc
