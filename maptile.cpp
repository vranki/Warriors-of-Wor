#include "maptile.h"
#include <C64Palette.h>
#include "sprite.h"
#include <QPainter>
#include <QImage>
#include <QDebug>

MapTile::MapTile(TilePos newPos) : QObject(), QGraphicsItem() {
    myPos = newPos;
    mn = ms = mw = me = 0;
    highlight = 0;
    _walls = 0;
    swapOtherImage = 0;
    tileContent = 0;
    setZValue(-10);
    for(int i=0;i<6;i++) {
        QColor c = C64Palette::color(6);
        if(i==2 || i==3) c=C64Palette::color(2);
        if(i==4 || i==5) c=C64Palette::color(1);

        nPixmap[i] = new QPixmap(Sprite::loadBitmap("wow-sprites/wall_n.png", c));
        wPixmap[i] = new QPixmap(Sprite::loadBitmap("wow-sprites/wall_w.png", c));
    }
    np.setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
    wp.setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
    np.setParentItem(this);
    wp.setParentItem(this);

    for(int i=2;i<6;i++) {
        QPixmap * pm = nPixmap[i];
        QImage im = pm->toImage();
        QPainter painter(&im);
        painter.begin(&im);
        int y=0;
        if(i % 2==0) y=1;
        for(int x=0;x<im.width();x=x+2) {
            painter.setPen(Qt::black);
            painter.drawRect(x,y,1,0);
            if(y==0) {
                y = 1;
            } else {
                y = 0;
            }
        }
        painter.end();
        pm->convertFromImage(im);
    }

    for(int i=2;i<6;i++) {
        QPixmap * pm = wPixmap[i];
        QImage im = pm->toImage();
        QPainter painter;
        painter.begin(&im);
        int y=0;
        if(i % 2==0) y=1;
        for(int x=0;x<im.height();x=x+1) {
            painter.setPen(Qt::black);
            painter.drawRect(y,x,0,0);
            if(y==0) {
                y = 1;
            } else {
                y = 0;
            }
        }
        painter.end();
        pm->convertFromImage(im);
    }

    fireTimer.setSingleShot(true);
    connect(&fireTimer, SIGNAL(timeout()), this, SLOT(endFire()));
    fireTimer.setInterval(1000);

    setMode(0);
}

MapTile::~MapTile() {
    for(int i=0;i<6;i++) {
        delete nPixmap[i];
        delete wPixmap[i];
    }
}

void MapTile::setNeightbors(MapTile *nn, MapTile *ns, MapTile *nw, MapTile *ne) {
    mn = nn;
    ms = ns;
    mw = nw;
    me = ne;
}

MapTile *MapTile::n() {
    if(_walls & MT_N) return 0;
    return mn;
}

MapTile *MapTile::e() {
    if(!me) return 0;
    if(me->walls() & MT_W) return 0;
    return me;
}

MapTile *MapTile::w() {
    if(_walls & MT_W) return 0;
    return mw;
}

MapTile *MapTile::s() {
    if(!ms) return 0;
    if(ms->walls() & MT_N) return 0;
    return ms;
}

MapTile *MapTile::tileTo(int dir) {
    if(dir==MT_N) return n();
    if(dir==MT_S) return s();
    if(dir==MT_W) return w();
    if(dir==MT_E) return e();
    return 0;
}


MapTile *MapTile::tileTo(QPoint dir) {
    if(dir.x() > 0) return e();
    if(dir.x() < 0) return w();
    if(dir.y() > 0) return s();
    if(dir.y() < 0) return n();
    return 0;
}

QPoint MapTile::position() {
    return myPos;
}

QPointF MapTile::coords() {
    return QPointF(myPos.x()*TILEW, myPos.y()*TILEH);
}

QRectF MapTile::boundingRect() const {
    return QRectF(0,0,TILEW, TILEH);
}

void MapTile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                    QWidget *widget) {
    //if(!highlight) return;
    if(content() & MT_CONTENT_FIRE)
        painter->setBrush(C64Palette::color(7));
    if(content())
        painter->drawRect(QRect(0,0,25,25));
}

void MapTile::setHighlight(bool hl) {
    highlight = hl;
    update();
}

int MapTile::walls() {
    return _walls;
}

void MapTile::setWalls(int nw) {
    _walls = nw;

    np.setVisible(_walls & MT_N);
    wp.setVisible(_walls & MT_W);
}

void MapTile::setMode(int m) {
    mode = m;
    np.setPixmap(*nPixmap[mode*2]);
    wp.setPixmap(*wPixmap[mode*2]);
}

void MapTile::setContent(int newContent)
{
    int oldContent = content();
    tileContent = newContent;
    if(!(oldContent & MT_CONTENT_FIRE) && newContent & MT_CONTENT_FIRE) {
        fireTimer.start();
    }
    if(oldContent != newContent) {
        qDebug() << Q_FUNC_INFO << "at" << position();

        emit contentChanged(tileContent);
        update();
    }
}

int MapTile::content()
{
    return tileContent;
}

void MapTile::changePattern() {
    if(swapOtherImage) {
        np.setPixmap(*nPixmap[mode*2]);
        wp.setPixmap(*wPixmap[mode*2]);
    } else {
        np.setPixmap(*nPixmap[mode*2+1]);
        wp.setPixmap(*wPixmap[mode*2+1]);
    }
    swapOtherImage = !swapOtherImage;
}

void MapTile::endFire()
{
    if(content() & MT_CONTENT_FIRE)
        setContent(content() - MT_CONTENT_FIRE);
}
