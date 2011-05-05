#include "garwor.h"
#include "player.h"

Garwor::Garwor(QObject *parent,
               PlayfieldInfo *pfinfo,
               SamplePlayer *smp) :
StealthCharacter(parent, pfinfo, smp)
{
    myColor = C64Palette::color(7);
    animationPixmaps.append(loadBitmap("wow-sprites/spr309E.bmp", myColor));
    animationPixmaps.append(loadBitmap("wow-sprites/spr311E.bmp", myColor));
    animationPixmaps.append(loadBitmap("wow-sprites/spr30DE.bmp", myColor));
    setPixmap(animationPixmaps.value(0));
    shootingProbability = 30;
    stealthProbability = 30;
    setCharacterSpeed(20);
}
