#ifndef BOMB_H
#define BOMB_H

#include <QObject>
#include <QTimer>
#include <sprite.h>
#include "playfield.h"
#include "sampleplayer.h"

class Bomb : public Sprite
{
    Q_OBJECT
public:
    Bomb(QObject *parent, PlayfieldInfo *pfinfo, SamplePlayer *smp, int distance);
    void tick(float dt);
    QRectF boundingRect() const;
private slots:
    void changeColor();
    void explode();
    void pollSituation();
private:
    PlayfieldInfo *playfield;
    SamplePlayer *samples;
    int mySound, currentFrame;
    QList<QPixmap> animationPixmaps;
    QTimer colorChangeTimer, explodeTimer, pollTimer;
    int explosionDistance;
};

#endif // BOMB_H
