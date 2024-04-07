#ifndef GAMEMODE_H
#define GAMEMODE_H

#include <QObject>
#include "player.h"
#include "playfield.h"
#include "sampleplayer.h"

class GameMode : public QObject {
    Q_OBJECT
public:
    explicit GameMode(QObject *parent, QList<Character*> &plrs);
    virtual void initGame(Playfield *f, SamplePlayer *s)=0;
    virtual QString name() const =0;
    virtual QString description() const =0;
    virtual void startGame()=0;
    virtual void startRound()=0;
signals:
    void showRoundEnd(Player *winner);
    void showGameEnd(Player *winner);
public slots:

protected:
    void setPlayersControllable(bool c);
    QList<Character*> &characters;
    Playfield *field;
    SamplePlayer *samples;
};

#endif // GAMEMODE_H
