#include "radar.h"

Radar::Radar(QObject *parent, QList<Character*> &plrs) : QObject(parent), QGraphicsItem(), characters(plrs) {
    connect(&updateTimer, SIGNAL(timeout()), this, SLOT(updateMap()));
    updateTimer.setSingleShot(false);
    updateTimer.start(200);
}

QRectF Radar::boundingRect() const {
    return QRectF(0,0,RADARW, RADARH);
}

void Radar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                QWidget *widget) {
    painter->setPen(Qt::NoPen);
    painter->setBrush(C64Palette::color(6));
    painter->drawRect(0, 1, 1, RADARH-1);
    painter->drawRect(RADARW-1, 1, 1, RADARH-1);
    painter->drawRect(1, 0, RADARW-2, 1);

    foreach(Character *c, characters) {
        QPointF pos = c->pos();
        painter->setBrush(c->color());
        int x = pos.x();
        int y = pos.y();
        x /= TILEW;
        y /= TILEH;

        painter->drawRect(x*8, y*6, 4, 4);
    }
}

void Radar::updateMap() {
    update();
}
