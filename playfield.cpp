#include "playfield.h"
#include "player.h"
#include "characters/enemycharacter.h"
#include <QFile>
#include <QTextStream>
#include "maptile.h"
Playfield::Playfield(QObject *parent, QList<Character*> &chars) : QObject(parent), _scene(), characters(chars) {
    for(int x=0;x < MAPW;x++)
        for(int y=0;y < MAPH; y++)
            mapTiles[x][y] = 0;

    _scene.setBackgroundBrush(QBrush(QColor("black")));
    levelNameItem = new QGraphicsSimpleTextItem(nullptr);
    levelNameItem->setBrush(C64Palette::color(2));
    _scene.addItem(levelNameItem);
    QFont fnt = QFont("Adore64");
    fnt.setPixelSize(6);
    levelNameItem->setFont(fnt);
    levelNameItem->setZValue(1);
    radar = new Radar(this, characters);
    radar->setPos(TILEW*4+TILEW/2, TILEH*7+8);
    _scene.addItem(radar);
    radar->setVisible(false);
    patternChangeTimer.setInterval(200);
    patternChangeTimer.setSingleShot(false);
    connect(&patternChangeTimer, SIGNAL(timeout()), this, SLOT(changePattern()));
    setMode(0);
    mapCount = 0;
    mapVisible = false;
}

Playfield::~Playfield() {
    delete levelNameItem;
}

int Playfield::loadMapData(const QString& filename) {
    mapFileName = filename;
    QFile f(mapFileName);
    int r = f.exists();
    Q_ASSERT(f.exists());
    return !r;
}

void Playfield::setMapName(const QString& name) {
    levelNameItem->setText(name);
    levelNameItem->setPos(TILEW*5, TILEH*7+3);
}

void Playfield::loadMap(int num) {
    //qDebug() << Q_FUNC_INFO << "num:" << num;
    QFile mapFile(mapFileName);
    Q_ASSERT(mapFile.exists());
    mapFile.open(QIODevice::ReadOnly);
    QString dataline = mapFile.readLine();
    mapCount = dataline.left(dataline.indexOf(' ')).toInt();
    Q_ASSERT(mapCount);

    for(int x=0;x < MAPW;x++) {
        for(int y=0;y < MAPH; y++) {
            MapTile *mt = tileAt(x,y);
            if(mt) {
                delete(mt);
                mapTiles[x][y] = 0;
            }
        }
    }
    warpTiles.clear();
    spawnPoints.clear();
    QString prevLine;
    int y=0, lineNumber=-1;
    int loadedMaps=-1;
    while(mapFile.canReadLine()) {
        QString line = mapFile.readLine();
        if(line.startsWith("MAP")) {
            lineNumber=0;
            loadedMaps++;
            qDebug() << Q_FUNC_INFO << "Next map: " << loadedMaps;
        } else {
  //          qDebug() << Q_FUNC_INFO << y << (y % 2);
            if(loadedMaps==num && (lineNumber % 2)) {
                if(line.endsWith('\n'))
                    line.chop(1);
                while(line.length() < MAPW*2+2)
                    line.append(' ');
//                qDebug() << Q_FUNC_INFO << "Loading this";
                qDebug() << Q_FUNC_INFO << "Line:" << line << y;
                for(int x = 0; x < MAPW; x++) {
                    QChar mapChar = line.at(1+x*2);
                    // First, figure out which walls this tile has:
                    int walls = 0;

                    if(line.at(x*2)=='|')
                        walls |= MapTile::MT_W;

                    if(!prevLine.isEmpty()) {
                        if(prevLine.at(1+x*2)=='-')
                            walls |= MapTile::MT_N;
                    }
                    // Ok, walls setup.
                    // Is this a regular or spawn tile?
                    bool isASpawnTile;
                    int spawnNumber = QString(mapChar).toInt(&isASpawnTile);
                    MapTile *mt = 0;
                    if(mapChar=='w') { // Warp tile
                        WarpTile *wt = new WarpTile(TilePos(x,y), this);
                        mt = wt;
                        warpTiles.append(wt);
                    } else if(isASpawnTile) {
                        SpawnTile *st = new SpawnTile(TilePos(x,y));
                        mt = st;
                        if(y==0) {
                            st->setSpawnDirection(MapTile::MT_S);
                            if(x < MAPW/2) {
                                st->setLivesPosition(QPointF(TILEW, 0));
                            } else {
                                st->setLivesPosition(QPointF(-TILEW, 0));
                            }
                        } else if(y==MAPH-1) {
                            st->setSpawnDirection(MapTile::MT_N);
                            if(x < MAPW/2) {
                                st->setLivesPosition(QPointF(TILEW, 0));
                            } else {
                                st->setLivesPosition(QPointF(-TILEW, 0));
                            }
                        } else if(x < MAPW/2) {
                            st->setSpawnDirection(MapTile::MT_E);
                            if(y < MAPH/2) {
                                st->setLivesPosition(QPointF(0, -TILEH));
                            } else {
                                st->setLivesPosition(QPointF(0, TILEH));
                            }
                        } else if(x >= MAPW/2) {
                            st->setSpawnDirection(MapTile::MT_W);
                            if(y < MAPH/2) {
                                st->setLivesPosition(QPointF(0, -TILEH));
                            } else {
                                st->setLivesPosition(QPointF(0, TILEH));
                            }
                        }
                        spawnPoints[spawnNumber] = st;
                    } else {
                        mt = new MapTile(TilePos(x,y));
                    }
                    mt->setWalls(walls);
                    if(mapChar=='s') mt->setContent(MapTile::MT_CONTENT_SAND);

                    // Add the new tile to table
                    mapTiles[x][y] = mt;
                    _scene.addItem(mt);
                    mt->setPos(mt->coords());
                }
                y++;
            }
            lineNumber++;
            prevLine = line;
        }
    }
    // Set the neighbor pointers
    for(int x=0;x<MAPW;x++) {
        for(int y=0;y<MAPH;y++) {
            MapTile *mt, *n, *s, *w, *e;
            n = s = w = e = 0;
            mt = mapTiles[x][y];
            Q_ASSERT(mt);
            if(x > 0)
                w = mapTiles[x-1][y];
            if(x < MAPW-1)
                e = mapTiles[x+1][y];
            if(y > 0)
                n = mapTiles[x][y-1];
            if(y < MAPH-1)
                s = mapTiles[x][y+1];
            mapTiles[x][y]->setNeightbors(n, s, w, e);
        }
    }
    Q_ASSERT(num < mapCount);
    setMapName("DUNGEON  " + QString::number(num+1));
    num = num % mapCount;
    // Set warp targets
    if(warpTiles.size()==2) {
        warpTiles[0]->setTarget(warpTiles[1]);
        warpTiles[1]->setTarget(warpTiles[0]);
    }
    Q_ASSERT(!spawnPoints.isEmpty());
    setMode(mode);
    setVisible(mapVisible);
}

QGraphicsScene* Playfield::scene() {
    return &_scene;
}

QPoint Playfield::tileSize() {
    return QPoint(TILEW, TILEH);
}

MapTile * Playfield::tileAt(QPointF coords) {
    return tileAt(TilePos(coords.x()/TILEW, coords.y()/TILEH));
}

MapTile * Playfield::tileAt(TilePos coords) {
    if(coords.x()<0 || coords.x()>MAPW || coords.y()<0 || coords.y()>MAPH)
        return 0;
    return mapTiles[coords.x()][coords.y()];
}

MapTile *Playfield::tileAt(int x, int y) {
    return tileAt(TilePos(x,y));
}

void Playfield::setVisible(bool visible) {
    mapVisible = visible;
    for(int x=0;x < MAPW;x++) {
        for(int y=0;y < MAPH; y++) {
            MapTile *mt = tileAt(x,y);
            if(mt)
                mt->setVisible(visible);
        }
    }
    levelNameItem->setVisible(visible);
    radar->setVisible(visible);
}

SpawnTile* Playfield::spawnPoint(int num) {
    return spawnPoints.value(num);
}

void Playfield::removeRandomWall() {
    int x = 2+qrand() % (MAPW-3);
    int y = 2+qrand() % (MAPH-4);
    MapTile *mt = tileAt(TilePos(x,y));
    Q_ASSERT(mt);
    if(qrand() % 2 == 1) {
        if(mt->walls() & MapTile::MT_W)
            mt->setWalls(mt->walls() - MapTile::MT_W);
    } else {
        if(mt->walls() & MapTile::MT_N)
            mt->setWalls(mt->walls() - MapTile::MT_N);
    }
}

void Playfield::tick(float dt) {

}

bool Playfield::tileContainsPlayer(MapTile *mt) {
    QList<QGraphicsItem*> items = mt->collidingItems(Qt::IntersectsItemBoundingRect);
    foreach(QGraphicsItem* item, items) {
        if(dynamic_cast<Player*> (item))
            return true;
    }
    return false;
}

bool Playfield::tileContainsEnemy(MapTile *mt) {
    QList<QGraphicsItem*> items = mt->collidingItems(Qt::IntersectsItemBoundingRect);
    foreach(QGraphicsItem* item, items) {
        if(dynamic_cast<EnemyCharacter*> (item))
            return true;
    }
    return false;
}

void Playfield::setMode(int m) {
    mode = m;
    if(mode==0) {
        patternChangeTimer.stop();
    } else {
        patternChangeTimer.start();
    }
    for(int x=0;x<MAPW;x++) {
        for(int y=0;y<MAPH;y++) {
            MapTile *mt = tileAt(QPoint(x,y));
            if(mt)
                mt->setMode(m);
        }
    }
}

MapTile *Playfield::randomTile(bool notCloseToCharacters) {
    //qDebug() << Q_FUNC_INFO << "notCloseToCharacters:" << notCloseToCharacters << " characters.size():" << characters.size();
    int x = 1+qrand() % (MAPW-2);
    int y = 1+qrand() % (MAPH-2);
    //qDebug() << "characters has size " << characters.size();
    if(notCloseToCharacters) {
        for(int minRange = 4; minRange >0;minRange--) {
            bool posOk = true;
            foreach(Character *c, characters) {
                if (!c) {
                    //qDebug() << "Error: Character in list is NULL";
                    continue;
                }
                Player *p = qobject_cast<Player*>(c);
                if(p) {
                    // successful only if character is a player
                    //if(!p->getControllable())
                    //    continue;
                    int dx = qAbs(x - p->currentTile()->position().x());
                    int dy = qAbs(y - p->currentTile()->position().y());
                    if(dx < minRange && dy < minRange)
                        posOk = false;
                }
            }
            if(posOk) {
                //qDebug() << "posOK at " << x << y;
                return tileAt(TilePos(x,y));
            }
            //qDebug() << "random position";
            x = 1+qrand() % (MAPW-2);
            y = 1+qrand() % (MAPH-2);
        }
    }
    //qDebug() << "returning " << x << y;
    return tileAt(TilePos(x,y));
}

void Playfield::setBackgroundColor(QColor c) {
    scene()->setBackgroundBrush(QBrush(c));
}

void Playfield::changePattern() {
    return;
    for(int x=0;x<MAPW;x++)
        for(int y=0;y<MAPH;y++) {
            MapTile *mt = tileAt(QPoint(x,y));
            mt->changePattern();
        }
}

void Playfield::updateContent()
{
    for(int x=0;x<MAPW;x++)
        for(int y=0;y<MAPH;y++) {
            MapTile *mt = tileAt(QPoint(x,y));
            mt->updateFireTile();
        }
}

bool Playfield::tileIsValidLocation(MapTile *mt) {
    bool isValid = true;
    if(!mt)
        return false;
    if(mt->position().x()<=0 || mt->position().x() > MAPW-1)
        isValid = false;
    if(mt->position().y()<=0 || mt->position().y() > MAPH-1)
        isValid = false;

    if(qobject_cast<WarpTile*>(mt))
        isValid = true;

    if(!isValid && mt) {
        qDebug() << Q_FUNC_INFO << "MT " << mt->position() << "ain't valid!";
    }
    return isValid;
}
