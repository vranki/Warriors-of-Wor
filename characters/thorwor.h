#ifndef THORWOR_H
#define THORWOR_H

#include <stealthcharacter.h>

class Thorwor : public StealthCharacter
{
    Q_OBJECT
public:
    explicit Thorwor(QObject *parent,
                     PlayfieldInfo *pfinfo,
                     SamplePlayer *smp);
};

#endif // THORWOR_H
