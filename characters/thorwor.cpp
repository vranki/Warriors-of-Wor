#include "thorwor.h"


Thorwor::Thorwor(QObject *parent,
               PlayfieldInfo *pfinfo,
               SamplePlayer *smp) :
StealthCharacter(parent, pfinfo, smp)
{
    myColor = C64Palette::color(10);
    animationPixmaps.append(loadBitmap("wow-sprites/spr315E.bmp", myColor));
    animationPixmaps.append(loadBitmap("wow-sprites/spr319E.bmp", myColor));
    animationPixmaps.append(loadBitmap("wow-sprites/spr31DE.bmp", myColor));
    setPixmap(animationPixmaps.value(0));
    shootingProbability = 10;
    stealthProbability = 20;
    setCharacterSpeed(20);
}
