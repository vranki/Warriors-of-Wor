#include "burwor.h"

Burwor::Burwor(QObject *parent,
               PlayfieldInfo *pfinfo,
               SamplePlayer *smp) :
EnemyCharacter(parent, pfinfo, smp)
{
    myColor = C64Palette::color(14);
    animationPixmaps.append(loadBitmap("wow-sprites/spr305E.bmp", myColor));
    animationPixmaps.append(loadBitmap("wow-sprites/spr301E.bmp", myColor));
    animationPixmaps.append(loadBitmap("wow-sprites/spr2FDE.bmp", myColor));
    setPixmap(animationPixmaps.value(0));
    shootingProbability = 50;
    setCharacterSpeed(20);
}
