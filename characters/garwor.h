#ifndef GARWOR_H
#define GARWOR_H

#include <QObject>
#include "stealthcharacter.h"

class Garwor : public StealthCharacter
{
    Q_OBJECT
public:
    explicit Garwor(QObject *parent,
                    PlayfieldInfo *pfinfo,
                    SamplePlayer *smp);

};

#endif // GARWOR_H
