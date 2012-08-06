#include "bomb.h"

Bomb::Bomb(QObject *parent, PlayfieldInfo *pfinfo, SamplePlayer *smp, int distance) : Sprite(parent)
{
    playfield = pfinfo;
    samples = smp;
    currentFrame = 0;
    explosionDistance = distance;
    animationPixmaps.append(loadBitmap("wow-sprites/bomb.bmp", C64Palette::color(0x0b)));
    animationPixmaps.append(loadBitmap("wow-sprites/bomb2.bmp", C64Palette::color(0x0b)));
    colorChangeTimer.setSingleShot(false);
    colorChangeTimer.setInterval(100);
    connect(&colorChangeTimer, SIGNAL(timeout()), this, SLOT(changeColor()));
    colorChangeTimer.start();
    changeColor();

    explodeTimer.setSingleShot(true);
    explodeTimer.setInterval(3000);
    connect(&explodeTimer, SIGNAL(timeout()), this, SLOT(explode()));
    explodeTimer.start();

    pollTimer.setSingleShot(false);
    pollTimer.setInterval(50);
    connect(&pollTimer, SIGNAL(timeout()), this, SLOT(pollSituation()));
    pollTimer.start();
}

QRectF Bomb::boundingRect() const {
    return QRectF(0,0,30, 30);
}

void Bomb::changeColor() {
    setPixmap(animationPixmaps.value(currentFrame));
    currentFrame++;
    if(currentFrame > animationPixmaps.size()-1)
        currentFrame = 0;
}

void Bomb::explode()
{
    qDebug() << Q_FUNC_INFO;
    explodeTimer.stop();
    pollTimer.stop();
    MapTile *mt = 0;
    MapTile *center = playfield->tileAt(pos() + QPointF(TILEW/2, TILEH/2));
    center->setContent(center->content() | MapTile::MT_CONTENT_FIRE);

    int distanceExploded = 1;
    mt = center;
    // Expand E
    while(distanceExploded < explosionDistance && mt->e()) {
        mt = mt->e();
        mt->setContent(mt->content() | MapTile::MT_CONTENT_FIRE);
        distanceExploded++;
    }

    distanceExploded = 1;
    mt = center;
    // Expand W
    while(distanceExploded < explosionDistance && mt->w()) {
        mt = mt->w();
        mt->setContent(mt->content() | MapTile::MT_CONTENT_FIRE);
        distanceExploded++;
    }
    distanceExploded = 1;
    mt = center;
    // Expand N
    while(distanceExploded < explosionDistance && mt->n()) {
        mt = mt->n();
        mt->setContent(mt->content() | MapTile::MT_CONTENT_FIRE);
        distanceExploded++;
    }
    distanceExploded = 1;
    mt = center;
    // Expand S
    while(distanceExploded < explosionDistance && mt->s()) {
        mt = mt->s();
        mt->setContent(mt->content() | MapTile::MT_CONTENT_FIRE);
        distanceExploded++;
    }
    colorChangeTimer.stop();
    playfield->updateContent();
    scene()->removeItem(this);
    disconnect(this);
    //deleteLater(); @todo crashes, delete properly
}

void Bomb::pollSituation()
{
    if(playfield->tileAt(pos() + QPointF(TILEW/2, TILEH/2))->content() & MapTile::MT_CONTENT_FIRE)
        explode();
}
