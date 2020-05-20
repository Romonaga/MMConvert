#-------------------------------------------------
#
# Project created by QtCreator 2020-04-25T11:51:34
#
#-------------------------------------------------

QT       += core gui sql

#QMAKE_CXXFLAGS += j8
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimediawidgets

TARGET = MMConvert
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mmconvert.cpp \
    tablemodel.cpp \
    mmconversioninfo.cpp \
    ffmpeginvoker.cpp \
    dialogfinddupes.cpp \
    dialogimportmovies.cpp \
    dialogmoviecompare.cpp \
    cboxsimpledelegate.cpp \
    filefindsimpledelegate.cpp \
    spinsimpledelegate.cpp \
    mmsettings.cpp \
    dialogmmsettings.cpp \
    mmdirwatcher.cpp \
    plexmediatablemodel.cpp \
    dialogplexdbscanner.cpp \
    dialogplayvideo.cpp

HEADERS += \
        mmconvert.h \
    tablemodel.h \
    mmconversioninfo.h \
    ffmpeginvoker.h \
    dialogfinddupes.h \
    dialogimportmovies.h \
    dialogmoviecompare.h \
    cboxsimpledelegate.h \
    filefindsimpledelegate.h \
    spinsimpledelegate.h \
    mmsettings.h \
    dialogmmsettings.h \
    mmdirwatcher.h \
    plexmediatablemodel.h \
    dialogplexdbscanner.h \
    mmconvertcommon.h \
    dialogplayvideo.h

FORMS += \
        mmconvert.ui \
    dialogfinddupes.ui \
    dialogimportmovies.ui \
    dialogmoviecompare.ui \
    dialogmmsettings.ui \
    dialogplexdbscanner.ui \
    dialogplayvideo.ui

#libav support
unix:!macx: LIBS += -lavformat
unix:!macx: LIBS += -lavcodec
unix:!macx: LIBS += -lavutil
#unix:!macx: LIBS += -lswscale


unix {
    target.path = /usr/local/bin
    INSTALLS += target
}

RESOURCES += \
    mmconvertresources.qrc
