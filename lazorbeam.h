#ifndef LAZORBEAM_H
#define LAZORBEAM_H
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPointF>
#include <QPoint>
#include <QDebug>
#include "playfieldinfo.h"
#include "sampleplayer.h"
#include <C64Palette.h>

class Character;

#define BEAM_LENGTH 8
#define BEAM_WIDTH 2

class LazorBeam : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    LazorBeam(QObject *parent, PlayfieldInfo *pfinfo, QPointF dir, SamplePlayer *smp, bool enemys);
    void tick(float dt);
    void hitSomething(Character *plr = 0);
    bool isEnemys();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
signals:
    void hitWall();
    void hitCharacter(Character *);
private:
    void checkCollision(QPointF pos);
    PlayfieldInfo *playfield;
    QPointF direction;
    SamplePlayer *samples;
    int mySound;
    bool enemyBeam;
};

#endif // LAZORBEAM_H
