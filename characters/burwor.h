#ifndef BURWOR_H
#define BURWOR_H

#include <QObject>
#include "enemycharacter.h"

class Burwor : public EnemyCharacter
{
    Q_OBJECT
public:
    explicit Burwor(QObject *parent,
                    PlayfieldInfo *pfinfo,
                    SamplePlayer *smp);
};

#endif // BURWOR_H
