#include "character.h"

Character::Character(QObject *parent,
                     PlayfieldInfo *pfinfo,
                     SamplePlayer *smp) :
Sprite(parent) {
    currentAnimationPixmap = 0;
    playfield = pfinfo;
    samples = smp;
    setTransformOriginPoint(10,10);
    connect(&animationTimer, SIGNAL(timeout()), this, SLOT(animationTimeout()));
    setOffset(2,2);
    killsPlayer = isEnemy = false;
    lazorBeam = 0;
    speedScale = 1;
    animationRate = 5000;
    setCharacterSpeed(50);
}
Character::~Character() {
}

void Character::setCharacterSpeed(float spd) {
    characterSpeed = spd;
    animationTimer.setInterval(animationRate/(characterSpeed*speedScale));
}

QRectF Character::boundingRect() const {
    return QRectF(0,0,30, 30);
}

void Character::tick(float dt) {
    if(!isVisible()) return;
    if(forcedmoveTarget) {
        spawnTick(dt);
        return;
    }

    if(lazorBeam)
        lazorBeam->tick(dt);

    if(!tiles[0]) tiles[0] = currentTile();
    if(!currentTile()) return; // In outer space

    if(!controllable) return; // controlDir = QPoint();
    QPoint newControlDir = controlDir;

    xaligned = false;
    yaligned = false;

    if(abs(pos().y() - currentTile()->coords().y()) < 1) {
        yaligned = true;
    }
    if(abs(pos().x() - currentTile()->coords().x()) < 1) {
        xaligned = true;
    }
    if(!yaligned) {
        newControlDir.setX(0);
        if(controlDir.x() > 0) {
            if(pos().y() < currentTile()->coords().y() + TILEH/2 && currentTile()->e()) {
                newControlDir.setY(-1);
            }else if(pos().y() > currentTile()->coords().y() + TILEH/2 && currentTile()->s() && currentTile()->s()->e()) {
                newControlDir.setY(1);
            }
        } else if(controlDir.x() < 0) {
            if(pos().y() < currentTile()->coords().y() + TILEH/2 && currentTile()->w()) {
                newControlDir.setY(-1);
            }else if(pos().y() > currentTile()->coords().y() + TILEH/2 && currentTile()->s() && currentTile()->s()->w()) {
                newControlDir.setY(1);
            }
        }
    }
    if(!xaligned) {
        newControlDir.setY(0);
        if(controlDir.y() > 0) {
            if(pos().x() < currentTile()->coords().x() + TILEW/2 && currentTile()->s()) {
                newControlDir.setX(-1);
            }else if(pos().x() > currentTile()->coords().x() + TILEW/2 && currentTile()->e() && currentTile()->e()->s()) {
                newControlDir.setX(1);
            }
        } else if(controlDir.y() < 0) {
            if(pos().x() < currentTile()->coords().x() + TILEW/2 && currentTile()->n()) {
                newControlDir.setX(-1);
            }else if(pos().x() > currentTile()->coords().x() + TILEW/2 && currentTile()->e() && currentTile()->e()->n()) {
                newControlDir.setX(1);
            }
        }
    }
    direction=QPoint(newControlDir * characterSpeed * speedScale);
    QPointF movement = QPointF((direction * dt).x(), (direction * dt).y());

    QPointF newPos = pos() + movement;

    if(!currentTile()->e() && newPos.x() > currentTile()->x()) {
        movement.setX(0);
        setPos(currentTile()->x(), pos().y());
    }
    if(!currentTile()->w() && newPos.x() < currentTile()->x()) {
        movement.setX(0);
        setPos(currentTile()->x(), pos().y());
    }
    if(!currentTile()->s() && newPos.y() > currentTile()->y()) {
        movement.setY(0);
        setPos(pos().x(), currentTile()->y());
    }
    if(!currentTile()->n() && newPos.y() < currentTile()->y()) {
        movement.setY(0);
        setPos(pos().x(), currentTile()->y());
    }
    if((newControlDir != spriteDir) || (characterStopped && !newControlDir.isNull())) {
        updateSprite(newControlDir);
    }
    moveBy(movement.x(), movement.y());

    // Check current tile(s)
    MapTile *topLeft = currentTile();
    MapTile *bottomRight = playfield->tileAt(QPointF(pos().x() + TILEW-1, pos().y() + TILEH-1));
    if(bottomRight == topLeft) bottomRight = 0;

    if(topLeft != tiles[0] && topLeft != tiles[1]) {
        tiles[0] = topLeft;
        tileEntered(topLeft);
    }

    if(bottomRight && bottomRight != tiles[0] && bottomRight != tiles[1]) {
        tiles[1] = bottomRight;
        tileEntered(bottomRight);
    }
}

void Character::spawnTick(float dt) {
    Q_ASSERT(isVisible());
    if(!isVisible()) return;
    direction = forcedMoveDir*characterSpeed*speedScale;
    moveBy((direction.x() * dt), (direction.y()*dt));
    Q_ASSERT(forcedmoveTarget);

    bool targetReached = false;
    if((forcedMoveDir.y() > 0 && pos().y() > forcedmoveTarget->pos().y()) ||
       (forcedMoveDir.y() < 0 && pos().y() < forcedmoveTarget->pos().y()) ||
       (forcedMoveDir.x() < 0 && pos().x() < forcedmoveTarget->pos().x()) ||
       (forcedMoveDir.x() > 0 && pos().x() > forcedmoveTarget->pos().x()))
        targetReached = true;
    if(targetReached) {
        setPos(forcedmoveTarget->pos());
        tiles[0] = forcedmoveTarget;
        tiles[1] = 0;
        emit forcedMoveTargetReached();
        tileEntered(forcedmoveTarget);
        forcedmoveTarget = 0;
        forcedMoveDir = QPoint();
    }
}


void Character::warpTileLeft() {
    setControllable(true);
    disconnect(this, SIGNAL(forcedMoveTargetReached()));
}

void Character::animationTimeout() {
}

void Character::updateSprite(QPoint dir) {
    if(!characterStopped && spriteDir==dir) return;

    if(!dir.isNull()) {
        tf.reset();
        spriteDir = dir;
        characterStopped = false;
    } else {
        characterStopped = true;
    }
    if(dir.y() > 0)
       tf.rotate(90);
    if(dir.y() < 0)
        tf.rotate(-90);
    if(dir.x() < 0) {
        tf.scale(-1,1);
    } else if(dir.x() > 0) {

    }
    setPixmap(animationPixmaps[currentAnimationPixmap].transformed(tf));
    if(dir.isNull()) {
        animationTimer.stop();
    } else if(!animationTimer.isActive()){
        animationTimer.start();
    }
}

void Character::setDirection(QPoint dir) {
    if(controlDir==dir) return;

    if((dir.x() > 0 && dir.y() > 0) || (dir.x() > 0 && dir.y()<0)) {
        if(controlDir.x()>0) {
            dir.setY(0);
        } else {
            dir.setX(0);
        }
    } else if((dir.x() < 0 && dir.y()>0) || (dir.x() < 0 && dir.y()<0)) {
        if(controlDir.x()<0) {
            dir.setY(0);
        } else {
            dir.setX(0);
        }
    }
    Q_ASSERT(!(dir.x() && dir.y()));

    controlDir = dir;
}

MapTile *Character::currentTile() {
    return playfield->tileAt(pos());
}

void Character::stopShooting() {
    shooting = false;
    animationTimeout();
}

void Character::deathBlink() {
    deathBlinkCounter++;
    setVisible(deathBlinkCounter % 2);
    if(deathBlinkCounter < 7) {
        QTimer::singleShot(200, this, SLOT(deathBlink()));
    } else {
        setPixmap(animationPixmaps[0]);
        setVisible(false);
        setControllable(false);
        Explosion *e = new Explosion(parent());
        e->setPos(pos());
        scene()->addItem(e);
        connect(e, SIGNAL(destroyed()), this, SIGNAL(killed()));
        samples->deathexplosion();
    }
}

void Character::resetCharacter() {
    tf.reset();
    tiles[0] = tiles[1] = 0;
    spriteDir = QPoint(1,0);
    setPixmap(animationPixmaps[0]);
    setControllable(false);
    setVisible(true);
    shooting = false;
    invulnerable = true;
    dead = false;
    characterStopped = true;
    deathBlinkCounter = 0;
    forcedMoveDir = QPoint(0,0);
    forcedmoveTarget = 0;
    animationTimer.stop();
    animationTimeout();
    if(lazorBeam)
        lazorBeam->hitSomething();
}

void Character::tileEntered(MapTile *mt) {
    if(!playfield->tileIsValidLocation(mt))
        killCharacter(true);

    if(qobject_cast<WarpTile*>(mt)) {
        WarpTile *wt = qobject_cast<WarpTile*>(mt);
        if(wt->isOpen()) {
            samples->enter();
            setPos(wt->warpTo());
            forcedMoveDir = wt->warpDirection();
            forcedmoveTarget = playfield->tileAt(wt->targetTile->position() + forcedMoveDir);
            connect(this, SIGNAL(forcedMoveTargetReached()), this, SLOT(warpTileLeft()));
            emit enteredWarp();
        }
    }
}

void Character::makeVulnerable() {
    invulnerable = false;
}


void Character::setControllable(bool ctrlable) {
/*    qDebug() << Q_FUNC_INFO << ctrlable;
    Q_ASSERT(!ctrlable);*/
    controllable = ctrlable;
    if(!controllable)
        animationTimer.stop();
}
void Character::killCharacter(bool blinkfirst) {
    if(lazorBeam)
        lazorBeam->hitSomething();

    dead = true;
    setControllable(false);
    if(blinkfirst) {
        deathBlinkCounter = 1;
    } else {
        deathBlinkCounter = 99;
    }
    deathBlink();
}

void Character::fireWeapon() {
    if(lazorBeam) return;
    if(dead) return;
    if(spriteDir.x() > 0 && !currentTile()->e())
        return;
    if(spriteDir.x() < 0 && !currentTile()->w() && pos().x() - currentTile()->coords().x() < TILEW/2)
        return;
    if(spriteDir.y() > 0 && !currentTile()->s())
        return;
    if(spriteDir.y() < 0 && !currentTile()->n() && pos().y() - currentTile()->coords().y() < TILEH/2)
        return;

    lazorBeam = new LazorBeam(this, playfield, QPointF(spriteDir*200), samples, isEnemy);
    lazorBeam->setPos(pos());
    if(spriteDir.y() < 0)
        lazorBeam->moveBy(0,-TILEH/2);
    if(spriteDir.y() > 0)
        lazorBeam->moveBy(0,TILEH/2);
    if(spriteDir.x() < 0)
        lazorBeam->moveBy(-TILEW/2,0);
    if(spriteDir.x() > 0)
        lazorBeam->moveBy(TILEH/2,0);
    lazorBeam->moveBy(TILEW/2, TILEH/2);
    scene()->addItem(lazorBeam);
    connect(lazorBeam, SIGNAL(hitWall()), this, SLOT(lazorDestroyed()));
    connect(lazorBeam, SIGNAL(hitCharacter(Character*)), this, SIGNAL(lazorHitCharacter(Character*)));
    shooting = true;
    QTimer::singleShot(100, this, SLOT(stopShooting()));
    animationTimeout();
}

void Character::lazorDestroyed() {
    if(!lazorBeam) return;
    scene()->removeItem(lazorBeam);
    lazorBeam->deleteLater();
    lazorBeam = 0;
}

QColor Character::color() {
    return myColor;
}
