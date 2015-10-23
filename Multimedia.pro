#-------------------------------------------------
#
# Project created by QtCreator 2015-10-12T20:53:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Multimedia
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    myvideowidget.cpp

HEADERS  += mainwindow.h \
    myvideowidget.h

FORMS    += mainwindow.ui

QT += multimedia #用于多媒体播放的模块

QT += multimediawidgets #用于视频播放的模块

RESOURCES += \
    resource.qrc

RC_FILE = app.rc

DISTFILES += \
    app.rc
