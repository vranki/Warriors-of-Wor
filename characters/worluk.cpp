#include "worluk.h"



Worluk::Worluk(QObject *parent,
               PlayfieldInfo *pfinfo,
               SamplePlayer *smp) :
    EnemyCharacter(parent, pfinfo, smp)
{
    myColor = C64Palette::color(0);
    animationPixmaps.append(QPixmap("wow-sprites/worluk1.png"));
    animationPixmaps.append(QPixmap("wow-sprites/worluk2.png"));
    setPixmap(animationPixmaps.value(0));
    shootingProbability = 0;
    setCharacterSpeed(100);
}

void Worluk::animationTimeout() {
    currentAnimationPixmap++;
    if(currentAnimationPixmap > 1)
        currentAnimationPixmap = 0;
    setPixmap(animationPixmaps[currentAnimationPixmap]);
    Character::animationTimeout();
}
