#-------------------------------------------------
#
# Project created by QtCreator 2016-10-18T16:58:48
#
#-------------------------------------------------

QT       += core gui \
            multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = team3
TEMPLATE = app


SOURCES += main.cpp \
    object.cpp \
    highscore.cpp \
    world.cpp \
    loadsave.cpp \
    collisiondetails.cpp \
    player.cpp \
    platform.cpp \
    collectible.cpp \
    coin.cpp  \
    enemy.cpp \
    mainwidget.cpp \
    titlescreen.cpp \
    highscorepage.cpp \
    playingscreen.cpp \
    endgame.cpp \
    endgameobject.cpp \
    pausescreen.cpp \
    mapmaker.cpp \
    helpscreen.cpp \
    savestatedialog.cpp

HEADERS  += mainwidget.h \
    object.h \
    highscore.h \
    world.h \
    loadsave.h \
    collisiondetails.h \
    player.h \
    platform.h \
    collectible.h \
    coin.h \
    enemy.h \
    titlescreen.h \
    highscorepage.h \
    playingscreen.h \
    endgame.h \
    endgameobject.h \
    pausescreen.h \
    mapmaker.h \
    helpscreen.h \
    savestatedialog.h

FORMS    += mainwidget.ui \
    titlescreen.ui \
    highscorepage.ui \
    endgame.ui \
    pausescreen.ui \
    mapmaker.ui \
    helpscreen.ui \
    savestatedialog.ui

QMAKE_CXXFLAGS += -std=c++11

RESOURCES += \
    resources.qrc
