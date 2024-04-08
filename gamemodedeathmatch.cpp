#include "gamemodedeathmatch.h"
#include "sampleplayer.h"

GameModeDeathmatch::GameModeDeathmatch(QObject *parent, QList<Character*> &plrs) : GameMode(parent, plrs) {
    winsNeeded = 3;
    gameRound = 0;
    gameLengthTimer.setSingleShot(true);
    connect(&gameLengthTimer, SIGNAL(timeout()), this, SLOT(gameLengthTimerExpired()));
    removeWallTimer.setSingleShot(false);
    connect(&removeWallTimer, SIGNAL(timeout()), this, SLOT(removeWallTimerExpired()));
}

void GameModeDeathmatch::initGame(Playfield *f, SamplePlayer *s) {
    field = f;
    samples = s;
    field->loadMapData("maps_wow.txt");
}

QString GameModeDeathmatch::name() const {
    return "DEATHMATCH";
}

QString GameModeDeathmatch::description() const {
    return "Survive deathmatch 3 times to win";
}

void GameModeDeathmatch::startGame() {
    foreach(Character *c, characters) {
        Player *p = qobject_cast<Player*>(c);
        if(p) {
            p->setWins(0);
            connect(p, SIGNAL(gameOver(Player*)), this, SLOT(gameOver(Player*)));
        }
    }
    startRound();
}

void GameModeDeathmatch::startRound() {
    Player *winner = 0;
    foreach(Character *c, characters) {
        Player *p = qobject_cast<Player*>(c);
        if(p) {
            if(p->numberOfWins() >= winsNeeded)
                winner = p;
            qDebug() << "player " << p->number() << " wins " << p->numberOfWins() << "of" << winsNeeded;
        }
    }
    if(winner) {
        qDebug() << "Game Over";
        emit showGameEnd(winner);
        return;
    }

    field->loadMap(gameRound);
    field->setVisible(true);
    foreach(Character *c, characters) {
        Player *p = qobject_cast<Player*>(c);
        if(p) {
            p->resetLives();
            p->spawnPlayer();
        }
    }
    samples->background();
    gameRound++;
    gameLengthTimer.start(3 * 60 * 1000);
}


void GameModeDeathmatch::gameOver(Player *p) {
    int playersAlive = 0;
    foreach(Character *c, characters) {
        Player *p = qobject_cast<Player*>(c);
        if(p) {
            if(p->livesLeft() >= 0)
                playersAlive++;
        }
    }
    if(characters.size() > 2 && playersAlive == 2) gameLengthTimerExpired();
    if(playersAlive > 1) return;
    Player *winner = 0;
    foreach(Character *c, characters) {
        Player *p = qobject_cast<Player*>(c);
        if(p) {
            if(p->livesLeft() >= 0) {
                p->setWins(p->numberOfWins() + 1);
                winner = p;
            }
            p->resetCharacter();
            p->setControllable(false);
            p->setVisible(false);
        }
    }
    Q_ASSERT(winner);
    samples->stopbackground();
    gameLengthTimer.stop();
    removeWallTimer.stop();
    emit showRoundEnd(winner);
}

void GameModeDeathmatch::gameLengthTimerExpired() {
    qDebug() << Q_FUNC_INFO;
    removeWallTimer.start(300);
    samples->stopbackground();
    samples->background(GS_BACKGROUND_1);
}

void GameModeDeathmatch::removeWallTimerExpired() {
    field->removeRandomWall();
}
