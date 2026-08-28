#-------------------------------------------------
#
# Project created by QtCreator 2010-09-08T19:24:11
#
#-------------------------------------------------

QT += core gui
QT += widgets

TARGET = wow
TEMPLATE = app

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += sdl2
    LIBS += -lSDL2_mixer
}

win32 {
    CONFIG += no_cwiid
    DEFINES += SDL_MAIN_HANDLED

    isEmpty(SDL2_DIR): SDL2_DIR = $$(SDL2_DIR)
    isEmpty(SDL2_MIXER_DIR): SDL2_MIXER_DIR = $$(SDL2_MIXER_DIR)

    isEmpty(SDL2_DIR) {
        error("Set SDL2_DIR to the SDL2 development package directory")
    }
    isEmpty(SDL2_MIXER_DIR) {
        error("Set SDL2_MIXER_DIR to the SDL2_mixer development package directory")
    }
    !exists($$SDL2_DIR/include/SDL.h) {
        error("SDL.h not found below SDL2_DIR: $$SDL2_DIR")
    }
    !exists($$SDL2_MIXER_DIR/include/SDL_mixer.h) {
        error("SDL_mixer.h not found below SDL2_MIXER_DIR: $$SDL2_MIXER_DIR")
    }

    contains(QMAKE_TARGET.arch, x86_64) {
        SDL2_ARCH = x64
    } else {
        SDL2_ARCH = x86
    }

    INCLUDEPATH += "$$SDL2_DIR/include" "$$SDL2_MIXER_DIR/include"
    LIBS += -L"$$SDL2_DIR/lib/$$SDL2_ARCH"
    LIBS += -L"$$SDL2_MIXER_DIR/lib/$$SDL2_ARCH"
    LIBS += -lSDL2 -lSDL2_mixer
}

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

SOURCES += main.cpp\
        mainwindow.cpp \
    playfield.cpp \
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
    radar.cpp \
    bomb.cpp \
    characters/character.cpp \
    characters/burwor.cpp \
    characters/worrior.cpp \
    characters/enemycharacter.cpp \
    characters/garwor.cpp \
    characters/stealthcharacter.cpp \
    characters/thorwor.cpp \
    characters/worluk.cpp \
    characters/wizard.cpp \
    gamemodebomberman.cpp

HEADERS  += mainwindow.h \
    playfield.h \
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
    radar.h \
    gameselectionmenu.h \
    roundendscreen.h \
    gameoverscreen.h \
    gamemodedeathmatch.h \
    gamemode.h \
    gamemodeclassic.h \
    gamemodebomberman.h \
    bomb.h \
    characters/character.h \
    characters/burwor.h \
    characters/worrior.h \
    characters/enemycharacter.h \
    characters/garwor.h \
    characters/stealthcharacter.h \
    characters/thorwor.h \
    characters/worluk.h \
    characters/wizard.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    maps_wow.txt \
    maps_bomberman.txt \
    README.md

have_cwiid {
    SOURCES += wiimote.cpp \
        wiimotefinder.cpp

    HEADERS += wiimote.h \
        wiimotefinder.h
}
