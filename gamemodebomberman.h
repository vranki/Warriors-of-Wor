#ifndef GAMEMODEBOMBERMAN_H
#define GAMEMODEBOMBERMAN_H

#include "gamemode.h"
#include <QString>

class GameModeBomberman : public GameMode
{
    Q_OBJECT
public:
    explicit GameModeBomberman(QObject *parent, QList<Character*> &plrs);
    virtual void initGame(Playfield *f, SamplePlayer *s);
    virtual QString name();
    virtual QString description();
    virtual void startGame();
    virtual void startRound();
signals:
    
private slots:
    virtual void gameOver(Player *p);
    void playerKilled();
};

#endif // GAMEMODEBOMBERMAN_H
