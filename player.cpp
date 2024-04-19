#include "player.h"

Player::Player(QObject *parent,
               PlayfieldInfo *pfinfo,
               SamplePlayer *smp,
               QColor color,
               int controllertype,
               int controllerid,
               int plnum) :
    Worrior(parent, pfinfo, smp, color, plnum)
{
    ctrlType = controllertype;
    ctrlId = controllerid;
    playfield = pfinfo;
    playerNumber = plnum;
    setControllable(false);
    setVisible(true);
    wins = 0;
    resetPlayer();
    connect(this, SIGNAL(killed()), this, SLOT(characterKilled()));
    connect(this, SIGNAL(lazorHitCharacter(Character*)), this, SLOT(lazorHitPlayerCharacterSlot(Character*)));
    spawnTimer.setSingleShot(true);
    connect(&spawnTimer, SIGNAL(timeout()), this, SLOT(leaveSpawnTile()));
}

int Player::controllerType() const {
    return ctrlType;
}

int Player::controllerId() const {
    return ctrlId;
}

void Player::characterKilled() {
    //qDebug() << Q_FUNC_INFO;
    lives--;
    playfield->spawnPoint(playerNumber)->setLivesLeft(lives);
    lazorDestroyed();
    if(lives >= 0) {
        resetCharacter();
        spawnPlayer();
    } else {
        setPos(0,0);
        setControllable(false);
        setVisible(false);
        emit gameOver(this);
    }
}

void Player::spawnPlayer() {
    //qDebug() << Q_FUNC_INFO;
    playfield->spawnPoint(playerNumber)->setUsedByPlayer(true);
    playfield->spawnPoint(playerNumber)->setPlayerImage(pixmap());
    playfield->spawnPoint(playerNumber)->setLivesLeft(lives);
    setPos(playfield->spawnPoint(playerNumber)->pos());
    setControllable(false);
    setVisible(true);
    allowSpawn();
    spawnTimer.start(10000);
}

int Player::livesLeft() const {
    //qDebug() << Q_FUNC_INFO;
    return lives;
}

int Player::number() const {
    //qDebug() << Q_FUNC_INFO;
    return playerNumber;
}

void Player::resetLives() {
    //qDebug() << Q_FUNC_INFO;
    lives = 2;
}

void Player::addLife() {
    //qDebug() << Q_FUNC_INFO;
    if(lives < 2) lives++;
}

void Player::resetPlayer() {
    //qDebug() << Q_FUNC_INFO;
    resetCharacter();
    resetLives();
    kills = 0;
}

void Player::lazorHitPlayerCharacterSlot(Character* other) {
    Player *otherPlayer = qobject_cast<Player*> (other);
    kills++;
    //qDebug() << Q_FUNC_INFO << kills;
}

int Player::numberOfWins() const {
    return wins;
}

void Player::setWins(int w) {
    wins = w;
}
