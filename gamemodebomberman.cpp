#include "gamemodebomberman.h"
#include "characters/worrior.h"

GameModeBomberman::GameModeBomberman(QObject *parent, QList<Character*> &plrs) :
    GameMode(parent, plrs)
{
}

void GameModeBomberman::initGame(Playfield *f, SamplePlayer *s) {
    field = f;
    samples = s;
}

QString GameModeBomberman::name()
{
    return "BOMBERMAN";
}

QString GameModeBomberman::description()
{
    return "BLAST THEM AWAY!";
}

void GameModeBomberman::startGame()
{
    foreach(Character *c, characters) {
        Player *p = qobject_cast<Player*>(c);
        if(p) {
            p->setWins(0);
            p->resetLives();
            p->setWeaponType(Worrior::WEAPON_BOMB);
            connect(p, SIGNAL(gameOver(Player*)), this, SLOT(gameOver(Player*)));
            connect(p, SIGNAL(killed()), this, SLOT(playerKilled()));
        } else {
            Q_ASSERT(false);
        }
    }
    startRound();
}

void GameModeBomberman::startRound()
{
    field->loadMap(0);
    field->setMapName("RADAR");
    field->setVisible(true);
    foreach(Character *c, characters) {
        Player *p = qobject_cast<Player*>(c);
        if(p) {
            p->resetCharacter();
            p->spawnPlayer();
        } else {
            Q_ASSERT(false);
        }
    }
    samples->background();
    setPlayersControllable(true);
}

void GameModeBomberman::gameOver(Player *p)
{
}

void GameModeBomberman::playerKilled()
{
}
