#-------------------------------------------------
#
# Project created by QtCreator 2010-09-08T19:24:11
#
#-------------------------------------------------

QT       += core gui

TARGET = wow
TEMPLATE = app
CONFIG   += link_pkgconfig
PKGCONFIG += cwiid sdl

LIBS += -lSDL_mixer

SOURCES += main.cpp\
        mainwindow.cpp \
    playfield.cpp \
    character.cpp \
    playfieldinfo.cpp \
    maptile.cpp \
    lazorbeam.cpp \
    wiimote.cpp \
    wiimotefinder.cpp \
    player.cpp \
    gamemenu.cpp \
    sampleplayer.cpp \
    C64Palette.cpp \
    explosion.cpp \
    spawntile.cpp \
    sprite.cpp \
    warptile.cpp \
    gameselectionmenu.cpp \
    roundendscreen.cpp \
    gameoverscreen.cpp \
    gamemodedeathmatch.cpp \
    gamemode.cpp \
    gamemodeclassic.cpp \
    burwor.cpp \
    worrior.cpp \
    enemycharacter.cpp \
    garwor.cpp \
    radar.cpp \
    stealthcharacter.cpp \
    thorwor.cpp \
    worluk.cpp \
    wizard.cpp

HEADERS  += mainwindow.h \
    playfield.h \
    character.h \
    playfieldinfo.h \
    maptile.h \
    lazorbeam.h \
    wiimote.h \
    wiimotefinder.h \
    player.h \
    gamemenu.h \
    sampleplayer.h \
    C64Palette.h \
    explosion.h \
    spawntile.h \
    sprite.h \
    warptile.h \
    gameselectionmenu.h \
    roundendscreen.h \
    gameoverscreen.h \
    gamemodedeathmatch.h \
    gamemode.h \
    gamemodeclassic.h \
    burwor.h \
    worrior.h \
    enemycharacter.h \
    garwor.h \
    radar.h \
    stealthcharacter.h \
    thorwor.h \
    worluk.h \
    wizard.h

FORMS    += mainwindow.ui
