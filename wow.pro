#-------------------------------------------------
#
# Project created by QtCreator 2010-09-08T19:24:11
#
#-------------------------------------------------

QT += core gui
QT += widgets

TARGET = wow
TEMPLATE = app
CONFIG   += link_pkgconfig
PKGCONFIG += sdl2

!contains(CONFIG, no_cwiid) {
    packagesExist(cwiid) {
        PKGCONFIG += cwiid
        DEFINES += HAVE_CWIID
        CONFIG += have_cwiid
    } else {
        message("cwiid not found; Wiimote support disabled")
    }
} else {
    message("cwiid disabled; Wiimote support disabled")
}

LIBS += -lSDL2_mixer

SOURCES += main.cpp\
        mainwindow.cpp \
    playfield.cpp \
    characters/character.cpp \
    playfieldinfo.cpp \
    maptile.cpp \
    lazorbeam.cpp \
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

have_cwiid {
    SOURCES += wiimote.cpp \
        wiimotefinder.cpp

    HEADERS += wiimote.h \
        wiimotefinder.h
}
