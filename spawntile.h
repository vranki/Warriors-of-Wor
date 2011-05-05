#ifndef SPAWNTILE_H
#define SPAWNTILE_H
#include "maptile.h"
#include <QPoint>
#include <QPixmap>
#include <QGraphicsPixmapItem>

class SpawnTile : public MapTile
{
public:
    SpawnTile(TilePos pos);
    void setSpawnDirection(int dir);
    QPoint direction();
    int directionEnum();
    virtual void setWalls(int nw);
    void setUsedByPlayer(bool used);
    void setPlayerImage(QPixmap pm);
    void setLivesLeft(int ll);
    void setLivesPosition(QPointF inc);
private:
    QPoint _direction;
    bool _used;
    int realWalls;
    QList<QGraphicsPixmapItem*> livesImages;
};

#endif // SPAWNTILE_H
