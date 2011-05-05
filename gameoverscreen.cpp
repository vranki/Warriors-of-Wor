#include "gameoverscreen.h"

GameOverScreen::GameOverScreen(QList<Character*> &plrs, Player * w) : QObject(), QGraphicsItem(), players(plrs), blinker(this) {
    colorCycle = true;
    blink = true;
    winner = w;
    connect(&blinker, SIGNAL(timeout()), this, SLOT(repaintMenu()));
    blinker.setSingleShot(false);
    blinker.start(200);
    QTimer::singleShot(2000, this, SLOT(endBlinking()));
    if(winner) {
        winner->setPos(148,64);
        winner->setVisible(true);
    }
    update();
}

QRectF GameOverScreen::boundingRect() const {
    return QRectF(-5,-5,325, 205);
}

void GameOverScreen::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QPen pen;
    QFont font = QFont("Adore64");
    font.setPointSize(6);
    pen.setBrush(QBrush(C64Palette::color(6)));
    painter->setPen(pen);
    painter->setFont(font);
    if(winner) {
        painter->drawText(50,32, "Congratulations, player " + QString::number(winner->number()+1));
    } else {
        painter->drawText(50,32, "Game Over");
    }
    int ypos = 156;
    int xpos = 64;
    foreach(Character *p, players) {
        qDebug() << "player " << p->pos();
        if(qobject_cast<Player*>(p) && p != winner) {
            p->setPos(xpos, ypos-16);
            p->setVisible(true);
            p->resetCharacter();
            p->setControllable(false);
            xpos += 24;
        }
    }
}

void GameOverScreen::repaintMenu() {
    colorCycle = !colorCycle;
}

void GameOverScreen::endBlinking() {
    blink = false;
}
