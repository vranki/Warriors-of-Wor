#ifndef GAMESELECTIONMENU_H
#define GAMESELECTIONMENU_H

#include <QGraphicsItem>
#include <QPainter>
#include <C64Palette.h>
#include "gamemodedeathmatch.h"
#include "gamemodeclassic.h"
#include <QFontDatabase>
#include <QFont>
#include <QTimer>

#define GAME_MODE_COUNT 2

class GameSelectionMenu : public QObject, public QGraphicsItem {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    GameSelectionMenu(QObject *parent, QList<Character*> &plrs);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
public slots:
    void buttonPressed(int button);
    void controlInput(QPoint dir);
    void repaintMenu();
signals:
    void gameModeSelected(GameMode * gm);
private:
    QGraphicsScene *scene;
    int colorCycle;
    GameMode *currentGameMode;
    int gameModeNumber;
    QList<Character*> &characters;
};

#endif // GAMESELECTIONMENU_H
