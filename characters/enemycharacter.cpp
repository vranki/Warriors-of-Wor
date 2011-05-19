#include "enemycharacter.h"

EnemyCharacter::EnemyCharacter(QObject *parent, PlayfieldInfo *pfinfo, SamplePlayer *smp) : Character(parent, pfinfo, smp) {
    setSpeedScale(1);
    killsPlayer = true;
    isEnemy = true;
    canShoot = true;
    shootingProbability = 0;
    animationRate = 10000;
}

void EnemyCharacter::tick(float dt) {
    Character::tick(dt);
    if(xaligned && yaligned) {
        QPoint cd = controlDir;
        MapTile* mt = playfield->tileAt(pos() + QPointF(TILEW/2, TILEH/2));
        Q_ASSERT(mt);

        if((cd.x() > 0 && !mt->e()) || (cd.x() < 0 && !mt->w()) ) {
//            qDebug() << Q_FUNC_INFO << "e or w wall, turning";
            if(qrand() & 1) {
                if(mt->s()) {
                    cd = QPoint(0,1);
                } else {
                    cd = QPoint(0,-1);
                }
            } else {
                if(mt->n()) {
                    cd = QPoint(0,-1);
                } else {
                    cd = QPoint(0,1);
                }
            }
        }
        if(
                (cd.y() > 0 && !mt->s()) ||
                (cd.y() < 0 && !mt->n())
                ) {
 //           qDebug() << Q_FUNC_INFO << "s or n wall, turning";
            if(qrand() & 1) {
                if(mt->e()) {
                    cd = QPoint(1,0);
                } else {
                    cd = QPoint(-1,0);
                }
            } else {
                if(mt->w()) {
                    cd = QPoint(-1,0);
                } else {
                    cd = QPoint(1,0);
                }
            }
        }

        if(cd == controlDir && lastTurnInTile != mt) { // No corned turn happened
            if((qrand() & 5)==0) { // Random turn
                if(cd.x() && mt->s() && (qrand() & 1)) {
                    cd = QPoint(0,1);
                } else if(cd.x() && mt->n() && (qrand() & 1)) {
                    cd = QPoint(0,-1);
                } else if(cd.y() && mt->w() && (qrand() & 1)) {
                    cd = QPoint(-1,0);
                } else if(cd.y() && mt->e() && (qrand() & 1)) {
                    cd = QPoint(1,0);
                }
            }
        }
        if(cd != controlDir) {
            lastTurnInTile = mt;
        }
        Q_ASSERT(cd.manhattanLength() > 0);
        setDirection(cd);
        if(shootingProbability > 0 && canShoot && (qrand() % shootingProbability==0))
            fireWeapon();
    }
    if(!dead) {
        QList<QGraphicsItem *> items = collidingItems();
        foreach(QGraphicsItem *item, items) {
            if(dynamic_cast<LazorBeam*>(item)) {
                LazorBeam *beam = dynamic_cast<LazorBeam*>(item);
                if(beam != lazorBeam && !beam->isEnemys()) {
                    beam->hitSomething(this);
                    killCharacter(false);
                }
            }
        }
    }
}

void EnemyCharacter::setSpeedScale(float ss) {
    animationTimer.setInterval(animationRate/(ss*characterSpeed));
}

void EnemyCharacter::animationTimeout() {
    currentAnimationPixmap++;
    if(currentAnimationPixmap > 2)
        currentAnimationPixmap = 0;
    setPixmap(animationPixmaps[currentAnimationPixmap].transformed(tf));
    Character::animationTimeout();
}

void EnemyCharacter::tileEntered(MapTile *mt) {
    Character::tileEntered(mt);
}
