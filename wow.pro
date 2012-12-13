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
    characters/character.cpp \
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
    characters/burwor.cpp \
    characters/worrior.cpp \
    characters/enemycharacter.cpp \
    characters/garwor.cpp \
    radar.cpp \
    characters/stealthcharacter.cpp \
    characters/thorwor.cpp \
    characters/worluk.cpp \
    characters/wizard.cpp \
    gamemodebomberman.cpp \
    bomb.cpp

HEADERS  += mainwindow.h \
    playfield.h \
    characters/character.h \
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
    characters/burwor.h \
    characters/worrior.h \
    characters/enemycharacter.h \
    characters/garwor.h \
    radar.h \
    characters/stealthcharacter.h \
    characters/thorwor.h \
    characters/worluk.h \
    characters/wizard.h \
    gamemodebomberman.h \
    bomb.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    maps_wow.txt \
    maps_bomberman.txt
