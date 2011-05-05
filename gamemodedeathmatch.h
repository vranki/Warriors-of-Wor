#ifndef GAMEMODEDEATHMATCH_H
#define GAMEMODEDEATHMATCH_H

#include <QObject>
//#include <QLine>
#include <QTimer>
#include <QList>
#include "gamemode.h"

class GameModeDeathmatch : public GameMode {
    Q_OBJECT
public:
    explicit GameModeDeathmatch(QObject *parent, QList<Character*> &plrs);
    virtual void initGame(Playfield *f, SamplePlayer *s);
    virtual QString name();
    virtual QString description();
    virtual void startGame();
    virtual void startRound();
signals:
    void showRoundEnd(Player *winner);
    void showGameEnd(Player *winner);
public slots:
    virtual void gameOver(Player *p);
    virtual void gameLengthTimerExpired();
    virtual void removeWallTimerExpired();
private:
    int gameRound;
    int winsNeeded;
    QTimer gameLengthTimer, removeWallTimer;
};

#endif // GAMEMODEDEATHMATCH_H
