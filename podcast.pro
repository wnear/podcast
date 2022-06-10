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
HEADERS += downloadmanager.h \
           downloadmanagerwgt.h \
           episodedata.h \
           episodemodel.h \
           episodewgt.h \
           episodelistwgt.h \
           localsave.h \
           mainwindow.h \
           opml.h \
           player.h \
           player_engine.h\
           podcast.h \
           podmodel.h \
           network.h \
           setting.h \
           subsonic.h \
           subsong.h \
           subpodcast.h \
           rssparser.h
SOURCES += downloadmanager.cc \
           downloadmanagerwgt.cc \
           episodedata.cc \
           episodemodel.cc \
           episodewgt.cc \
           episodelistwgt.cc \
           localsave.cc \
           main.cc \
           mainwindow.cc \
           opml.cc \
           player.cc \
           player_engine.cc\
           podcast.cc \
           podmodel.cc \
           network.cc \
           setting.cc \
           subsonic.cc \
           subsong.cc \
           subpodcast.cc \
           rssparser.cc
