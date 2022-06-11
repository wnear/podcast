######################################################################
# Automatically generated by qmake (3.1) Mon Jun 6 10:22:08 2022
######################################################################

QT += widgets network xml multimedia

CONFIG += qt

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

CONFIG += link_pkgconfig

PKGCONFIG += spdlog
CONFIG += C++20

HEADERS += downloadmanager.h \
           downloadmanagerwgt.h \
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
           opml.h \
           player.h \
           player_engine.h\
           podcast.h \
           podmodel.h \
           rssparser.h \
           setting.h \
           subpodcast.h \
           subsong.h \
           subsonic.h \



SOURCES += downloadmanager.cc \
           downloadmanagerwgt.cc \
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
           opml.cc \
           player.cc \
           player_engine.cc\
           podcast.cc \
           podmodel.cc \
           rssparser.cc \
           setting.cc \
           subpodcast.cc \
           subsong.cc \
           subsonic.cc \
