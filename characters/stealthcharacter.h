#ifndef STEALTHCHARACTER_H
#define STEALTHCHARACTER_H

#include "enemycharacter.h"

class StealthCharacter : public EnemyCharacter
{
    Q_OBJECT
public:
    explicit StealthCharacter(QObject *parent,
                              PlayfieldInfo *pfinfo,
                              SamplePlayer *smp);

signals:

public slots:
    virtual void tileEntered(MapTile *mt);
protected:
    bool seesPlayer();
    bool stealthMode; // AKA invisible
    int stealthProbability; // Less is more
};

#endif // STEALTHCHARACTER_H
