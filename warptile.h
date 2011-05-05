#ifndef WARPTILE_H
#define WARPTILE_H

#include "maptile.h"
#include "sprite.h"
#include "playfieldinfo.h"
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
#include <QDebug>

class WarpTile : public MapTile {
Q_OBJECT

public:
    WarpTile(TilePos pos, PlayfieldInfo *pfi);
    void setTarget(WarpTile *wt);
    QPointF warpTo();
    QPoint warpDirection();
    bool isOpen();
    WarpTile *targetTile;
public slots:
    void reopen();
private:
    QGraphicsPixmapItem holePixmap;
    QTimer closedTimer;
    bool holeOpen;
    PlayfieldInfo *playfield;
};

#endif // WARPTILE_H
