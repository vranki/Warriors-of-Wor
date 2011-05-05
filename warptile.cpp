#include "warptile.h"

WarpTile::WarpTile(TilePos pos, PlayfieldInfo *pfi) : MapTile(pos),  holePixmap(this) {
    targetTile = 0;
    playfield = pfi;
    holeOpen = true;
    QTransform tf;
    if(pos.x() > MAPW/2)
        tf.scale(-1,1);
    holePixmap.setPixmap(Sprite::loadBitmap("wow-sprites/warphole.bmp", 2).transformed(tf));
    connect(&closedTimer, SIGNAL(timeout()), this, SLOT(reopen()));
}

void WarpTile::setTarget(WarpTile *wt) {
    targetTile = wt;
    qDebug() << Q_FUNC_INFO << position() << wt->position();
}

QPointF WarpTile::warpTo() {
    if(!holeOpen)
        return pos();
    Q_ASSERT(targetTile);
    holePixmap.setVisible(false);
    holeOpen = false;
    closedTimer.start(5000);
    targetTile->warpTo();
    if(position().x() < MAPW/2) {
        MapTile *mt = playfield->tileAt(position() + QPoint(1,0));
        mt->setWalls(mt->walls() | MT_W);
    } else {
        setWalls(walls() | MT_W);
    }
    return targetTile->pos();
}

void WarpTile::reopen() {
    holePixmap.setVisible(true);
    holeOpen = true;
    if(position().x() < MAPW/2) {
        MapTile *mt = playfield->tileAt(position() + QPoint(1,0));
        if(mt->walls() & MT_W) mt->setWalls(mt->walls() - MT_W);
    } else {
        if(walls() & MT_W) setWalls(walls() - MT_W);
    }
}

bool WarpTile::isOpen() {
    return holeOpen;
}

QPoint WarpTile::warpDirection() {
    if(position().x() > MAPW/2) return QPoint(1,0);
    return QPoint(-1,0);
}
