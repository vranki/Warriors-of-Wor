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
    CONFIG   += link_pkgconfig
    PKGCONFIG += cwiid sdl2
    LIBS += -lSDL2_mixer
}

win32 {
    DEFINES += WOW_NO_CWIID SDL_MAIN_HANDLED

    isEmpty(SDL2_DIR) {
        SDL2_DIR = $$(SDL2_DIR)
    }
    isEmpty(SDL2_MIXER_DIR) {
        SDL2_MIXER_DIR = $$(SDL2_MIXER_DIR)
    }
    isEmpty(SDL2_DIR) {
        SDL2_CANDIDATES = $$PWD/.deps/SDL2 $$files($$PWD/.deps/SDL2-*)
        for(SDL2_CANDIDATE, SDL2_CANDIDATES) {
            isEmpty(SDL2_DIR):exists($$SDL2_CANDIDATE/include/SDL.h) {
                SDL2_DIR = $$SDL2_CANDIDATE
            }
        }
    }
    isEmpty(SDL2_MIXER_DIR) {
        SDL2_MIXER_CANDIDATES = $$PWD/.deps/SDL2_mixer $$files($$PWD/.deps/SDL2_mixer-*)
        for(SDL2_MIXER_CANDIDATE, SDL2_MIXER_CANDIDATES) {
            isEmpty(SDL2_MIXER_DIR):exists($$SDL2_MIXER_CANDIDATE/include/SDL_mixer.h) {
                SDL2_MIXER_DIR = $$SDL2_MIXER_CANDIDATE
            }
        }
    }

    !isEmpty(SDL2_DIR) {
        INCLUDEPATH += $$SDL2_DIR/include $$SDL2_DIR/include/SDL2
        LIBS += -L$$SDL2_DIR/lib -L$$SDL2_DIR/lib/x64
    } else {
        DEFINES += WOW_NO_SDL
    }

    !isEmpty(SDL2_MIXER_DIR) {
        INCLUDEPATH += $$SDL2_MIXER_DIR/include $$SDL2_MIXER_DIR/include/SDL2
        LIBS += -L$$SDL2_MIXER_DIR/lib -L$$SDL2_MIXER_DIR/lib/x64
    }

    !contains(DEFINES, WOW_NO_SDL) {
        LIBS += -lSDL2 -lSDL2_mixer
    }
}

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
