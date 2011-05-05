#ifndef WIZARD_H
#define WIZARD_H

#include "enemycharacter.h"

class Wizard : public EnemyCharacter
{
    Q_OBJECT
public:
    explicit Wizard(QObject *parent,
                    PlayfieldInfo *pfinfo,
                    SamplePlayer *smp);

signals:

public slots:
    virtual void tileEntered(MapTile *mt);
private:
    void teleport();
    int tilesWalked;
};

#endif // WIZARD_H
