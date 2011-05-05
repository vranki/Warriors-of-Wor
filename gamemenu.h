#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <QGraphicsItem>
#include <QPainter>
#include "player.h"
#include <C64Palette.h>
#include <QFont>
#include <QTimer>

class PlayerSelectionMenu : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    PlayerSelectionMenu(QList<Character*> &plrs);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
public slots:
    void repaintMenu();
private:
    QGraphicsScene *scene;
    int colorCycle;
    QList<Character*> &players;
};

#endif // GAMEMENU_H
