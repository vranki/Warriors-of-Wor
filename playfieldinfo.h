#ifndef PLAYFIELDINFO_H
#define PLAYFIELDINFO_H

#include <QPoint>
#include <QPointF>

#include "maptile.h"
class SpawnTile;

class PlayfieldInfo
{
public:
    PlayfieldInfo();
    virtual MapTile * tileAt(QPointF coords)=0;
    virtual MapTile * tileAt(TilePos coords)=0;
    virtual MapTile * randomTile(bool notCloseToCharacters=false)=0;
    virtual SpawnTile *spawnPoint(int num)=0;
    virtual bool tileContainsPlayer(MapTile *mt)=0;
    virtual bool tileContainsEnemy(MapTile *mt)=0;
    virtual bool tileIsValidLocation(MapTile *mt)=0;
};

#endif // PLAYFIELDINFO_H
