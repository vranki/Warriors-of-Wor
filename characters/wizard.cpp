#include "wizard.h"

Wizard::Wizard(QObject *parent,
               PlayfieldInfo *pfinfo,
               SamplePlayer *smp) :
    EnemyCharacter(parent, pfinfo, smp)
{
    myColor = C64Palette::color(0);
    animationPixmaps.append(loadBitmap("wow-sprites/spr321E.bmp", C64Palette::color(0x0E)));
    animationPixmaps.append(loadBitmap("wow-sprites/spr325E.bmp", C64Palette::color(0x0E)));
    animationPixmaps.append(loadBitmap("wow-sprites/spr329E.bmp", C64Palette::color(0x0E)));
    setPixmap(animationPixmaps.value(0));
    shootingProbability = 0;
    setCharacterSpeed(90);
    tilesWalked = 0;
}

void Wizard::tileEntered(MapTile *mt) {
    tilesWalked++;
    if(tilesWalked==5)
        fireWeapon();
    if(tilesWalked==10) {
        tilesWalked = 0;
        teleport();
    }
}

void Wizard::teleport() {
    MapTile *mt = playfield->randomTile(true);
    setPos(mt->x(), mt->y());
}
