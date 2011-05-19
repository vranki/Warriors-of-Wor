#ifndef ENEMYCHARACTER_H
#define ENEMYCHARACTER_H

#include <QObject>
#include "character.h"

class EnemyCharacter : public Character
{
    Q_OBJECT
public:
    EnemyCharacter(QObject *parent, PlayfieldInfo *pfinfo, SamplePlayer *smp);
    virtual void tick(float dt);
    void setSpeedScale(float ss);
public slots:
    virtual void animationTimeout();
    virtual void tileEntered(MapTile *mt);
protected:
    bool canShoot;
    int shootingProbability; // Less = more
    MapTile* lastTurnInTile; // In which tile turned last time (to prevent turning many times)
};

#endif // ENEMYCHARACTER_H
