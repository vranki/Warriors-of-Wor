#ifndef WORRIOR_H
#define WORRIOR_H

#include "character.h"

class Worrior : public Character
{
    Q_OBJECT

public:


    explicit Worrior(QObject *parent, PlayfieldInfo *pfinfo, SamplePlayer *smp, QColor color, int playerNum);
    virtual void tick(float dt);
    void allowSpawn();

signals:

public slots:
    virtual void setDirection(QPoint dir);
    void leaveSpawnTile();
    virtual void resetCharacter();
    void fire(int buttons);
    void spawnTileLeft();
    void animationTimeout();
private:
    int playerNumber;
    bool spawned;
    bool canspawn;
};

#endif // WORRIOR_H
