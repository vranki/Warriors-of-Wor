#ifndef GAMEMODECLASSIC_H
#define GAMEMODECLASSIC_H
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include "gamemode.h"
#include "characters/enemycharacter.h"
#include "characters/burwor.h"
#include "characters/garwor.h"
#include "characters/thorwor.h"
#include "characters/worluk.h"
#include "characters/wizard.h"
#include "playfield.h"

#define ENEMY_COUNT 6

enum GameState {
    GS_Intro = 0,
    GS_Normal,
    GS_Worluk,
    GS_WorlukDeath,
    GS_Wizard,
    GS_WizardDeath
};

class GameModeClassic : public GameMode
{
    Q_OBJECT
public:
    GameModeClassic(QObject *parent, QList<Character*> &plrs);
    ~GameModeClassic();
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
    void characterKilled();
    void nextIntroPhase();
    void worlukEscaped();
    void playerKilled();
    void worlukKilledBlink();
    void wizardKilledBlink();
private:
    void spawnNewBurwor();
    void spawnNewGarwor();
    void spawnNewThorwor();
    void spawnWorluk();
    void spawnWizard();
    void setupNewEnemy(EnemyCharacter *c);
    void killAllEnemies();
    void setPlayersControllable(bool c);
    void setGameState(GameState newState);
    void awardBonusPlayer();
    QList<EnemyCharacter*> enemyCharacters();
    int gameRound;
    int monstersSpawned;
    int burwors;
    int garwors;
    int thorwors;
    QTimer introTimer;
    int introPhase; // 0=nothing, 1=get ready 2=..go
    QGraphicsPixmapItem *getready, *go, *dsd, *bonusPlayer;
    bool worlukKilled, doubleScoreDungeon, bonusPlayerGiven;
    Worluk *worluk;
    Wizard *wizard;
    QTimer worlukKilledTimer, wizardKilledTimer;
//    int enemySpawningState; // 0 - normal enemies, 1 - worluk, 2 - wizard, 3- finished
    GameState gameState;
};

#endif // GAMEMODECLASSIC_H
