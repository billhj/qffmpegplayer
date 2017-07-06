#-------------------------------------------------
#
# Project created by QtCreator 2017-06-28T15:23:07
#
#-------------------------------------------------

QT       += core gui multimedia opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#CONFIG += console

TARGET = VRPlayer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ffmpegengine.cpp \
    qtaudioplayer.cpp \
    qtvideoplayer.cpp \
    videothread.cpp \
    qopenglvideoRenderer.cpp \
    Shader.cpp \
    qwidgetplayer.cpp

HEADERS  += mainwindow.h \
    ffmpegengine.h \
    qtaudioplayer.h \
    qtvideoplayer.h \
    videothread.h \
    qopenglvideoRenderer.h \
    Shader.h \
    qwidgetplayer.h

FORMS    += mainwindow.ui

LIBS += -L$$PWD/ffmpeg/lib/ -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lpostproc -lswresample -lswscale  \
        -L$$PWD/SDL2/lib/x64/ -lSDL2 -lSDL2main  \
        -L$$PWD/gl/lib/x64/ -lfreeglut -lglew32s #-lglew32s

INCLUDEPATH += $$PWD/ffmpeg/include/ \
                $$PWD/SDL2/include/ \
                $$PWD/gl/include/

DISTFILES += \
    shader.frag \
    shader.vert
