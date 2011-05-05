#ifndef WORLUK_H
#define WORLUK_H

#include "enemycharacter.h"

class Worluk : public EnemyCharacter
{
    Q_OBJECT
public:
    explicit Worluk(QObject *parent,
                    PlayfieldInfo *pfinfo,
                    SamplePlayer *smp);
    virtual void animationTimeout();
};

#endif // WORLUK_H
