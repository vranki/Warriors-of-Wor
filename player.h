#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QGraphicsItem>
#include <QTimer>
#include "worrior.h"
#include "playfieldinfo.h"
#include "C64Palette.h"

class Player : public Worrior {
    Q_OBJECT

public:
    explicit Player(QObject *parent,
                    PlayfieldInfo *pfinfo,
                    SamplePlayer *smp,
                    QColor color,
                    int controllertype,
                    int controllerid,
                    int plnum);
    int controllerType();
    int controllerId();
    int livesLeft(); // -1 = totally dead
    int numberOfKills();
    int number();
    int numberOfWins();
    void setWins(int w);
    void resetPlayer();
    void resetLives();
    void addLife();
signals:
    void gameOver(Player *);
public slots:
    void spawnPlayer();
    void characterKilled();
    void lazorHitPlayerCharacterSlot(Character* chr);
private:
    int ctrlType;
    int ctrlId;
    int lives, kills, wins;
    PlayfieldInfo *playfield;
    int playerNumber;
    QTimer spawnTimer;
};

#endif // PLAYER_H
