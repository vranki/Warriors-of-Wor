#include "stealthcharacter.h"

StealthCharacter::StealthCharacter(QObject *parent,
                                   PlayfieldInfo *pfinfo,
                                   SamplePlayer *smp) :
    EnemyCharacter(parent, pfinfo, smp)
{
    stealthMode = false;
    stealthProbability = 0;
    connect(&checkTimer, SIGNAL(timeout()), this, SLOT(checkIfSeesPlayer()));
    checkTimer.setInterval(100);
    checkTimer.setSingleShot(false);
    checkTimer.start();
}

void StealthCharacter::tileEntered(MapTile *mt) {
    EnemyCharacter::tileEntered(mt);
    bool oldStealthMode = stealthMode;
    if(stealthProbability > 0 && qrand()%stealthProbability==0) {
        if(!stealthMode) {
            if(!seesPlayer())
                stealthMode = true;
        } else {
            stealthMode = false;
        }

    }
    if(stealthMode != oldStealthMode)
        samples->enemyVisible();
    if(stealthMode) {
        setOpacity(0.0);
    } else {
        setOpacity(1);
    }
    canShoot = !stealthMode;
}

bool StealthCharacter::seesPlayer() {
    MapTile *mt = currentTile();
    do {
        if(playfield->tileContainsPlayer(mt))
            return true;
        mt = mt->n();
    } while(mt);
    mt = currentTile();
    do {
        if(playfield->tileContainsPlayer(mt))
            return true;
        mt = mt->e();
    } while(mt);
    mt = currentTile();
    do {
        if(playfield->tileContainsPlayer(mt))
            return true;
        mt = mt->s();
    } while(mt);
    mt = currentTile();
    do {
        if(playfield->tileContainsPlayer(mt))
            return true;
        mt = mt->w();
    } while(mt);
    mt = currentTile();

    return false;
}

void StealthCharacter::checkIfSeesPlayer() {
    if(!stealthMode)
        return;
    if(!seesPlayer())
        return;
    samples->enemyVisible();
    setOpacity(1);
    canShoot = true;
    stealthMode = false;
}
