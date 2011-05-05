#include "roundendscreen.h"

RoundEndScreen::RoundEndScreen(QList<Character*> &plrs, Player * w) : QObject(), QGraphicsItem(), players(plrs), blinker(this) {
    colorCycle = true;
    blink = true;
    winner = w;
    newCoin = 0;
    for(int i=0;i < players.size();i++) {
        Player *p = qobject_cast<Player*>(plrs[i]);
        if(p) {
            QList<QGraphicsPixmapItem*> row;
            for(int j=0;j < 10;j++) {
                QGraphicsPixmapItem* item = new QGraphicsPixmapItem(this);
                item->setPixmap(Sprite::loadBitmap("wow-sprites/coin.bmp", 7));
                item->setZValue(2);
                item->setVisible(p->numberOfWins() > j);
                if(p==winner && (j+1) == p->numberOfWins())
                    newCoin = item;
                row.append(item);
            }
            playerCoins.append(row);
        }
    }

    connect(&blinker, SIGNAL(timeout()), this, SLOT(repaintMenu()));
    blinker.setSingleShot(false);
    blinker.start(200);
    QTimer::singleShot(2000, this, SLOT(endBlinking()));
    update();
}

QRectF RoundEndScreen::boundingRect() const {
    return QRectF(-5,-5,325, 205);
}

void RoundEndScreen::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                           QWidget *widget) {

    QPen pen;
    C64Palette palette;
    QFont font = QFont("Adore64");
    font.setPointSize(6);
    pen.setBrush(QBrush(palette.color(6)));
    painter->setPen(pen);
    painter->setFont(font);
    int ypos = 16;
    int xpos = 48;
    foreach(Character *c, players) {
        Player *p = qobject_cast<Player*>(c);
        if(p) {
            int coinsToShow = p->numberOfWins();
            p->setPos(xpos, ypos-16);
            p->setVisible(true);
            for(int i=0;i < 10;i++) {
                QGraphicsPixmapItem* item = playerCoins[p->number()][i];
                Q_ASSERT(item);
                item->setPos(xpos + 32+24*i, ypos-14);
            }
            ypos += 24;
        }
    }
}

void RoundEndScreen::repaintMenu() {
    colorCycle = !colorCycle;
    newCoin->setVisible(colorCycle || !blink);
}

void RoundEndScreen::endBlinking() {
    blink = false;
}
