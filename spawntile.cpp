#include "spawntile.h"

SpawnTile::SpawnTile(TilePos pos) : MapTile(pos) {
    _direction = QPoint(0,0);
    _used = 0;
    for(int i=0; i < 10; i++) {
        QGraphicsPixmapItem *li = new QGraphicsPixmapItem(this);
        li->setVisible(false);
        li->setOffset(2,2);
        livesImages.append(li);
    }
}

void SpawnTile::setSpawnDirection(int dir){
    if(dir==MT_N)
        _direction = QPoint(0,-1);
    if(dir==MT_E)
        _direction = QPoint(1,0);
    if(dir==MT_S)
        _direction = QPoint(0,1);
    if(dir==MT_W)
        _direction = QPoint(-1,0);
    int walls = MT_N | MT_S | MT_E | MT_W;
    walls -= dir;
    setWalls(walls);
}

QPoint SpawnTile::direction(){
    return _direction;
}

int SpawnTile::directionEnum() {
    if(_direction.x() > 0) return MT_E;
    if(_direction.x() < 0) return MT_W;
    if(_direction.y() > 0) return MT_S;
    if(_direction.y() < 0) return MT_N;
    return 0;
}

void SpawnTile::setWalls(int nw) {
    realWalls = nw;
    if(_used) {
        int walls = MT_N | MT_S | MT_E | MT_W;
        MapTile::setWalls(walls);
        if(e())
            e()->setWalls(e()->walls() | MT_W);
        if(s())
            s()->setWalls(s()->walls() | MT_N);
    } else {
        MapTile::setWalls(realWalls);
    }
}

void SpawnTile::setUsedByPlayer(bool used) {
    _used = used;
    setWalls(realWalls);
    if(!_used) setLivesLeft(0);
}

void SpawnTile::setPlayerImage(QPixmap pm) {
    for(int i=0;i<livesImages.size();i++) {
        livesImages[i]->setPixmap(pm);
    }
}

void SpawnTile::setLivesLeft(int ll) {
    for(int i=0;i<livesImages.size();i++) {
        livesImages[i]->setVisible(i < ll);
    }
}

void SpawnTile::setLivesPosition(QPointF inc) {
    QPointF start = pos() + inc;
    for(int i=0;i<livesImages.size();i++) {
        livesImages[i]->setPos(start);
        start +=inc;
    }
}
