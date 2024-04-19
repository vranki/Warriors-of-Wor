#include "worrior.h"

Worrior::Worrior(QObject *parent, PlayfieldInfo *pfinfo, SamplePlayer *smp, QColor color, int playerNum) :
    Character(parent, pfinfo, smp)
{
    //qDebug() << Q_FUNC_INFO << "PlayerNumer: " << playerNum;
    playerNumber = playerNum;
    myColor = color;
    animationPixmaps.append(loadBitmap("wow-sprites/spr2E9E.bmp", myColor));
    animationPixmaps.append(loadBitmap("wow-sprites/spr2EDE.bmp", myColor));
    animationPixmaps.append(loadBitmap("wow-sprites/spr2F1E.bmp", myColor));
    animationPixmaps.append(loadBitmap("wow-sprites/spr2EDE.bmp", myColor));
    animationPixmaps.append(loadBitmap("wow-sprites/spr2F5E.bmp", myColor));
    setPixmap(animationPixmaps.value(0));
    lazorBeam = 0;

}


void Worrior::leaveSpawnTile() {
    if(!canspawn || spawned || forcedmoveTarget) return;
    forcedMoveDir = playfield->spawnPoint(playerNumber)->direction();
    forcedmoveTarget = playfield->tileAt(playfield->spawnPoint(playerNumber)->position() + forcedMoveDir);
    connect(this, SIGNAL(forcedMoveTargetReached()), this, SLOT(spawnTileLeft()));
    samples->spawn();
}

void Worrior::setDirection(QPoint dir) {
    if(canspawn && !spawned && playfield->spawnPoint(playerNumber)->direction() == dir) {
        leaveSpawnTile();
        return;
    }
    Character::setDirection(dir);
}

void Worrior::resetCharacter() {
    //qDebug() << Q_FUNC_INFO << "PlayerNumer: " << playerNumber;
    if(playfield->spawnPoint(playerNumber))
        setPos(playfield->spawnPoint(playerNumber)->pos());
    spawned = false;
    spawned = false;
    canspawn = false;
    setControllable(false);
    Character::resetCharacter();
}

void Worrior::tick(float dt) {
    Character::tick(dt);


    // Handle death
    if(!dead) {
        Q_ASSERT(isVisible() || !controllable);
        QList<QGraphicsItem *> items = collidingItems();
        bool killPlayer = false;
        foreach(QGraphicsItem *item, items) {
            if(dynamic_cast<LazorBeam*>(item)) {
                LazorBeam *beam = dynamic_cast<LazorBeam*>(item);
                if(beam != lazorBeam && !invulnerable) { // Hit by someone else's lazor!
                    emit wasHitBy(beam);
                    beam->hitSomething(this);
                    killPlayer = true;
                }
            } else if(dynamic_cast<Character*>(item)) {
                Character *c = dynamic_cast<Character*>(item);
                if(c->killsPlayer) {
                    if((c->direction().x() > 0 && c->pos().x() < pos().x()) ||
                            (c->direction().x() < 0 && c->pos().x() > pos().x()) ||
                            (c->direction().y() > 0 && c->pos().y() < pos().y()) ||
                            (c->direction().y() < 0 && c->pos().y() > pos().y())
                            )
                        killPlayer = true;
                }
            }
        }
        if(killPlayer) {
            samples->death();
            if(lazorBeam) lazorBeam->hitSomething();
            killCharacter(true);
        }
    }
}

void Worrior::fire(int buttons) {
    if(!controllable) return;
    if(buttons != 1) return;
    Q_ASSERT(isVisible()); // Wtf,this happens
    fireWeapon();
}


void Worrior::spawnTileLeft() {
    canspawn = false;
    spawned = true;

    setControllable(true);
    QTimer::singleShot(500, this, SLOT(makeVulnerable()));
    disconnect(this, SIGNAL(forcedMoveTargetReached()));
}

void Worrior::allowSpawn() {
    canspawn = true;
}


void Worrior::animationTimeout() {
    if(shooting) {
        setPixmap(animationPixmaps[4].transformed(tf));
        return;
    }
    currentAnimationPixmap++;
    if(currentAnimationPixmap > 3)
        currentAnimationPixmap = 0;
    setPixmap(animationPixmaps[currentAnimationPixmap].transformed(tf));
    Character::animationTimeout();
}
