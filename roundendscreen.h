#ifndef ROUNDENDSCREEN_H
#define ROUNDENDSCREEN_H


#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <C64Palette.h>
#include <QFontDatabase>
#include "sprite.h"
#include "QPixmap"
#include <QFont>
#include <QTimer>
#include <QList>
#include "player.h"

class RoundEndScreen: public QObject, public QGraphicsItem {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    RoundEndScreen(QList<Character*> &plrs, Player *w);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
public slots:
    void repaintMenu();
    void endBlinking();
private:
    QGraphicsScene *scene;
    bool colorCycle, blink;
    QList<Character*> players;
    QGraphicsPixmapItem *newCoin;
    QList< QList<QGraphicsPixmapItem*> > playerCoins;
    Player *winner;
    QTimer blinker;
};

#endif // ROUNDENDSCREEN_H
