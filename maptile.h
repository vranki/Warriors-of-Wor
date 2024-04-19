#ifndef MAPTILE_H
#define MAPTILE_H

#include <QPoint>
#include <QPointF>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QObject>
#include <QTimer>

typedef QPoint TilePos;
#define MAPW 13
#define MAPH 8
#define TILEW 24
#define TILEH 24
#define WALLW 2
#define WALLH 1

class MapTile : public QObject, public QGraphicsItem {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    static const int MT_W=1;
    static const int MT_E=1<<1;
    static const int MT_N=1<<2;
    static const int MT_S=1<<3;

    static const int MT_CONTENT_FIRE=1;
    static const int MT_CONTENT_BLOCK=1<<1;
    static const int MT_CONTENT_SAND=1<<2;


    MapTile(TilePos);
    ~MapTile();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setNeightbors(MapTile *nn, MapTile *ns, MapTile *nw, MapTile *ne);
    MapTile *tileTo(const int dir) const;
    MapTile *tileTo(const QPoint& dir) const;
    bool canWalkTo(const QPoint& dir) const;
    MapTile *n() const;
    MapTile *e() const;
    MapTile *w() const;
    MapTile *s() const;
    TilePos position() const;
    QPointF coords() const;
    void setHighlight(bool hl);
    int walls() const;
    virtual void setWalls(int nw);
    void setMode(int m);
    void setContent(int newContent);
    int content() const;
public slots:
    void changePattern();
    void updateFireTile();
signals:
    void contentChanged(int newContent);
private slots:
    void endFire();

private:
    TilePos myPos;
    MapTile *mn, *ms, *mw, *me;
    bool highlight;
    int _walls;
    QGraphicsPixmapItem np, wp, fireItem, sandItem;
    QPixmap *nPixmap[6], *wPixmap[6], fireCenterPixmap, fireHorizontalPixmap, fireVerticalPixmap, sandPixmap;
    int mode; // 0=normal, 1=worluk, 2=wizard
    bool swapOtherImage;
    bool fireTile;
    int tileContent;
    QTimer fireTimer;
};

#endif // MAPTILE_H
