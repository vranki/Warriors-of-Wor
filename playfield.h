#ifndef PLAYFIELD_H
#define PLAYFIELD_H

#include <QObject>
#include <QDebug>
#include <QGraphicsScene>
#include <QVector>
#include <QList>
#include <QPoint>
#include <QString>
#include <QGraphicsSimpleTextItem>
#include "C64Palette.h"
#include "playfieldinfo.h"
#include "maptile.h"
#include "spawntile.h"
#include "warptile.h"
#include "radar.h"

class Character;

class Playfield : public QObject, public PlayfieldInfo
{
    Q_OBJECT

public:
    explicit Playfield(QObject *parent, QList<Character*> &chars);
    ~Playfield();
    void loadMap(int num);
    QGraphicsScene* scene();
    QMap<int, SpawnTile*> spawnPoints;
    QList<WarpTile*> warpTiles;
    QPoint tileSize();
    virtual MapTile * tileAt(QPointF coords);
    virtual MapTile * tileAt(TilePos coords);
    void setVisible(bool visible);
    virtual SpawnTile* spawnPoint(int num);
    void removeRandomWall();
    void tick(float dt);
    virtual bool tileContainsPlayer(MapTile *mt);
    virtual bool tileContainsEnemy(MapTile *mt);
    virtual void setMapName(QString name);
    virtual bool tileIsValidLocation(MapTile *mt);
    void setMode(int m);
    virtual MapTile *randomTile(bool notCloseToCharacters=false); // On playfield
    void setBackgroundColor(QColor c);
signals:

public slots:
    void changePattern();
private:
    QGraphicsScene _scene;
//    QVector<QVector<MapTile*> > mapTiles;
    MapTile *mapTiles[MAPW][MAPH];
    QList<Character*> &characters;
    QGraphicsSimpleTextItem *levelNameItem;
    Radar *radar;
    int mode;
    QTimer patternChangeTimer;
};

#endif // PLAYFIELD_H
