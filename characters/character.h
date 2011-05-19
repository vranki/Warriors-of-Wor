#ifndef CHARACTER_H
#define CHARACTER_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPointF>
#include <QPoint>
#include <QTimer>
#include <QList>
#include <QDebug>
#include <QImage>
#include <QQueue>
#include <QGraphicsScene>
#include "playfieldinfo.h"
#include "lazorbeam.h"
#include "sampleplayer.h"
#include "explosion.h"
#include "spawntile.h"
#include "warptile.h"

class Character : public Sprite {
    Q_OBJECT
public:
//    static const int C_KILLS_PLAYER=1;
//    static const int C_IS_ENEMY=1<<1;
public:
    Character(QObject *parent, PlayfieldInfo *pfinfo, SamplePlayer *smp);
    ~Character();
    virtual void tick(float dt);
    MapTile *currentTile();
    QRectF boundingRect() const;
    QColor color();
    bool killsPlayer, isEnemy;
    void setCharacterSpeed(float spd);
    QPointF direction();
public slots:
    virtual void animationTimeout();
    virtual void setDirection(QPoint dir);
    void stopShooting();
    void deathBlink();
    virtual void resetCharacter();
    void makeVulnerable();
    void warpTileLeft();
    void setControllable(bool ctrlable);
    void killCharacter(bool blinkfirst);
    virtual void fireWeapon();
    void lazorDestroyed();

signals:
    void killed();
    void wasHitBy(QGraphicsItem *item);
    void lazorHitCharacter(Character *chr);
    void forcedMoveTargetReached();
    void enteredWarp();
protected:
    void updateSprite(QPoint dir);
    void spawnTick(float dt);
    virtual void tileEntered(MapTile *mt);
    QPointF _direction;
    QPoint controlDir;
    QPoint spriteDir;
    QList<QPixmap> animationPixmaps;
    QTimer animationTimer;
    int currentAnimationPixmap;
    QTransform tf;
    PlayfieldInfo *playfield;
    SamplePlayer *samples;
    bool shooting;
    int deathBlinkCounter;
    MapTile *tiles[2];
    bool invulnerable;
    QPoint forcedMoveDir;
    MapTile *forcedmoveTarget;
    bool controllable;
    bool xaligned;
    bool yaligned;
    float characterSpeed;
    float speedScale;
    // int flags;
    bool dead; // True during blink & explosion
    LazorBeam *lazorBeam;
    bool characterStopped;
    QColor myColor;
    float animationRate; // Larger = slower
};
#endif // CHARACTER_H
