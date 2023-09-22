######################################################################
# Automatically generated by qmake (3.1) Wed Jun 15 14:15:35 2022
######################################################################

QT += widgets network xml multimedia core opengl openglwidgets sql


CONFIG += link_pkgconfig
CONFIG += qt
CONFIG += C++17
CONFIG += debug_and_release

PKGCONFIG += spdlog

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
           #episodelistwgt.h \
           episodemodel.h \
           episodepropertymanager.h \
           episodewidgetitem.h \
           global.h \
           jsonop.h \
           log.h \
           mainwindow.h \
           opmlparser.h \
           player_engine.h \
           playercontrolwidget.h \
           podcast.h \
           podcastchannel.h \
           podmodel.h \
           rssparser.h \
           setting.h \
           utils.h \
           sqlmanager.h \

SOURCES += downloadmanager.cc \
           downloadmanagerwgt.cc \
           episode_detail_wgt.cc \
           episode_treemodel.cc \
           episode_treeview.cc \
           episode_treewidget.cc \
           episodedata.cc \
           #episodelistwgt.cc \
           episodemodel.cc \
           episodepropertymanager.cpp \
           episodewidgetitem.cc \
           global.cc \
           jsonop.cc \
           log.cc \
           main.cc \
           mainwindow.cc \
           opmlparser.cc \
           player_engine.cc \
           playercontrolwidget.cc \
           podcast.cc \
           podcastchannel.cc \
           podmodel.cc \
           rssparser.cc \
           setting.cc \
           utils.cc \
           sqlmanager.cc \

RESOURCES += \
    $$PWD/resources/sqlfiles.qrc \

