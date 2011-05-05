#include "lazorbeam.h"

LazorBeam::LazorBeam(QObject *parent,
                     PlayfieldInfo *pfinfo,
                     QPointF dir,
                     SamplePlayer *smp,
                     bool enemys) : QObject(parent)
{
    playfield = pfinfo;
    direction = dir;
    enemyBeam = enemys;
    samples = smp;
    if(!enemyBeam) {
        mySound = samples->shoot();
    } else {
        mySound = samples->enemyShoot();
    }

//    setRect(0,0,10,2);
    if(dir.y()) {
        resetTransform();
        rotate(90);
    }
    Q_ASSERT(!dir.isNull());
}

QRectF LazorBeam::boundingRect() const
{
    return QRectF(0,0,BEAM_LENGTH, BEAM_WIDTH);
}

void LazorBeam::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen;
    QBrush brush(C64Palette::color(2));
    brush.setStyle(Qt::SolidPattern);
    pen.setBrush(brush);
    painter->setPen(pen);
    painter->setBrush(brush);
    if(isEnemys()) {
        brush.setStyle(Qt::SolidPattern);
        pen.setBrush(brush);
        painter->setPen(pen);
        painter->setBrush(brush);
        painter->drawRect(0, 0, BEAM_LENGTH/4, BEAM_WIDTH/2);
        painter->drawRect(BEAM_LENGTH/4, BEAM_WIDTH/2, BEAM_LENGTH/4, BEAM_WIDTH/2);
        painter->drawRect(BEAM_LENGTH/4*2, 0, BEAM_LENGTH/4, BEAM_WIDTH/2);
        painter->drawRect(BEAM_LENGTH/4*3, BEAM_WIDTH/2, BEAM_LENGTH/4, BEAM_WIDTH/2);
    } else {
        painter->drawRect(0, 0, BEAM_LENGTH, BEAM_WIDTH);
    }
}

void LazorBeam::tick(float dt) {
    QPointF movement = direction*dt;
    checkCollision(pos());
    moveBy(movement.x(), movement.y());
    checkCollision(pos());
}

void LazorBeam::checkCollision(QPointF pos) {
    MapTile *mt = playfield->tileAt(pos);
    if(!mt) {
        hitSomething();
        return;
    }

    if(direction.x() > 0 && !mt->e() && pos.x() - mt->coords().x() > TILEW/2) {
        hitSomething();
    } else if(direction.x() < 0) {
        if((!mt->w() && pos.x() - mt->coords().x() < 2)
                || (!mt->e() && pos.x() - mt->coords().x() > TILEW/2)) {
            hitSomething();
        }
    } else if(direction.y() > 0 && !mt->s() && pos.y() - mt->coords().y() > TILEH/2) {
        hitSomething();
    } else if(direction.y() < 0) {
        if((!mt->n() && pos.y() - mt->coords().y() < 2)
                || (!mt->s() && pos.y() - mt->coords().y() > TILEH/2)) {
            hitSomething();
        }
    }
}

void LazorBeam::hitSomething(Character *plr) {
    if(mySound > 0)
        samples->stopSound(mySound);
    mySound = 0;
    if(plr)
        emit hitCharacter(plr);
    emit hitWall();
}

bool LazorBeam::isEnemys() {
    return enemyBeam;
}
