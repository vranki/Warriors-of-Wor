#include "gamemodeclassic.h"

GameModeClassic::GameModeClassic(QObject *parent, QList<Character*> &plrs) : GameMode(parent, plrs) {
    gameRound = 0;
    monstersSpawned = 0;
    introTimer.setSingleShot(true);
    connect(&introTimer, SIGNAL(timeout()), this, SLOT(nextIntroPhase()));
    connect(&worlukKilledTimer, SIGNAL(timeout()), this, SLOT(worlukKilledBlink()));
    worlukKilledTimer.setInterval(200);
    worlukKilledTimer.setSingleShot(false);
    connect(&wizardKilledTimer, SIGNAL(timeout()), this, SLOT(wizardKilledBlink()));
    wizardKilledTimer.setInterval(5000);
    wizardKilledTimer.setSingleShot(false);
    getready = go = dsd = bonusPlayer = 0;
    worlukKilled = doubleScoreDungeon = false;
    bonusPlayerGiven = false;
}

GameModeClassic::~GameModeClassic() {
    killAllEnemies();
}

void GameModeClassic::killAllEnemies() {
    foreach(EnemyCharacter *c, enemyCharacters()) {
        characters.removeOne(c);
        delete c;
    }
}

void GameModeClassic::initGame(Playfield *f, SamplePlayer *s) {
    field = f;
    samples = s;
}

QString GameModeClassic::name() {
    return "CLASSIC";
}

QString GameModeClassic::description() {
    return "Kill all enemies";
}

void GameModeClassic::startGame() {
    worlukKilled = doubleScoreDungeon = false;

    foreach(Character *c, characters) {
        Player *p = qobject_cast<Player*>(c);
        if(p) {
            p->setWins(0);
            p->resetLives();
            connect(p, SIGNAL(gameOver(Player*)), this, SLOT(gameOver(Player*)));
            connect(p, SIGNAL(killed()), this, SLOT(playerKilled()));
        } else {
            Q_ASSERT(false);
        }
    }
    introPhase = 0;
    setGameState(GS_Intro);
}

void GameModeClassic::nextIntroPhase() {
    Q_ASSERT(gameState == GS_Intro);
    introPhase++;
    qDebug() << Q_FUNC_INFO << introPhase;
    setPlayersControllable(false);
    if(worlukKilled)
        doubleScoreDungeon = true;

    if(introPhase==1) { // Getready
        Q_ASSERT(!getready);
        getready = new QGraphicsPixmapItem(0, field->scene());
        getready->setPixmap(QPixmap("wow-sprites/getready.png"));
        getready->setZValue(100);
        samples->stopbackground();
        samples->introLong2();
        introTimer.start(4300);
    } else if(introPhase==2) { // Go
        Q_ASSERT(getready);
        if(doubleScoreDungeon) {
            delete getready;
            getready = 0;
            Q_ASSERT(!dsd);
            dsd = new QGraphicsPixmapItem(0, field->scene());
            dsd->setPixmap(QPixmap("wow-sprites/doublescoredungeon.png"));
            dsd->setZValue(100);
            field->setVisible(false);
            foreach(Character *c, characters)
                c->setVisible(false);
        } else {
            Q_ASSERT(!go);
            go = new QGraphicsPixmapItem(0, field->scene());
            go->setPixmap(QPixmap("wow-sprites/go.png"));
            go->setZValue(100);
        }
        if(bonusPlayerGiven) {
            Q_ASSERT(!bonusPlayer);
            bonusPlayer = new QGraphicsPixmapItem(0, field->scene());
            bonusPlayer->setPixmap(QPixmap("wow-sprites/bonus_player.png"));
            bonusPlayer->setZValue(100);
            bonusPlayerGiven = false;
        }
        introTimer.start(4000);
    } else if(introPhase==3) {
        if(getready)
            delete getready;
        if(go)
            delete go;
        if(dsd)
            delete dsd;
        if(bonusPlayer)
            delete bonusPlayer;

        field->setVisible(true);
        foreach(Character *c, characters)
            c->setVisible(true);
        getready = go = dsd = bonusPlayer = 0;
        introPhase = 0;
        startRound();
    }
}

void GameModeClassic::startRound() {
    wizard = 0;
    worluk = 0;
    field->loadMap(gameRound);
    if(gameRound==0) field->setMapName("RADAR");
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
    gameRound++;
    setGameState(GS_Normal);
}

void GameModeClassic::gameOver(Player *p) {
    int playersAlive = 0;
    foreach(Character *c, characters) {
        Player *p = qobject_cast<Player*>(c);
        if(p) {
            if(p->livesLeft() >= 0)
                playersAlive++;
        }
    }
    if(!playersAlive) {
        killAllEnemies();
        samples->stopbackground();
        emit showGameEnd(0);
    }
}

/*
Normally, each dungeon starts filled with six Burwors.
In the first dungeon, killing the last Burwor will make a Garwor appear;
in the second, the last two Burwors are replaced by Garwors when killed; and so on.

From the sixth dungeon on, a Garwor will replace every Burwor when killed.
On every screen, killing a Garwor causes a Thorwor to appear.
There will never be more than six enemies on the screen at once.
From the second dungeon on, after the last Thorwor is killed, a Worluk will appear and
try to escape through one of the side doors, ending the level.
Killing the Worluk turns the next level into a "Double Score Dungeon."

The Wizard of Wor will appear in or after the 3rd dungeon once the Worluk has been killed
or has escaped (depending on how many men you have lost). After a few seconds the
Wizard will disappear and teleport across the dungeon, gradually approaching a Worrior.
The Wizard remains in the dungeon until he shoots a Worrior or is killed.
Killing the Wizard of Wor will also create a double score dungeon for the next dungeon.
*/
void GameModeClassic::characterKilled() {
    qDebug() << Q_FUNC_INFO;
    Character *c = qobject_cast<Character*>(sender());
    Q_ASSERT(c);
    Burwor *burwor = qobject_cast<Burwor*>(sender());
    Garwor *garwor = qobject_cast<Garwor*>(sender());
    Thorwor *thorwor = qobject_cast<Thorwor*>(sender());

    Q_ASSERT(characters.contains(c));
    characters.removeOne(c);
    delete c;
    QList<EnemyCharacter*> enemies = enemyCharacters();
    if(burwor) {
        if(garwors < gameRound && enemies.size()<gameRound)
            spawnNewGarwor();
    } else if(garwor) {
        spawnNewThorwor();
    } else if(worluk) {
        samples->stopbackground();
        worlukKilled = true;
        worluk = 0;
        setGameState(GS_WorlukDeath);
        worlukKilledTimer.start();
        field->setMode(1);
    } else if(wizard) {
        wizard = 0;
        field->setMode(2);
        wizardKilledTimer.start();
        setGameState(GS_WizardDeath);
    }
    if(enemyCharacters().isEmpty() && gameState == GS_Normal) {
        setGameState(GS_Worluk);

    }
}

QList<EnemyCharacter*> GameModeClassic::enemyCharacters() {
    QList<EnemyCharacter*> enemies;
    foreach(Character *c, characters) {
        EnemyCharacter *ec = qobject_cast<EnemyCharacter*>(c);
        if(ec) {
            enemies.append(ec);
        }
    }
    return enemies;
}

void GameModeClassic::setPlayersControllable(bool controllable) {
    qDebug() << Q_FUNC_INFO << controllable;
    foreach(Character *c, characters) {
        Player *p = qobject_cast<Player*>(c);
        if(p) {
            p->setControllable(controllable);
        }
    }
}

void GameModeClassic::awardBonusPlayer() {
    qDebug() << Q_FUNC_INFO;
    bonusPlayerGiven = true;
    foreach(Character *c, characters) {
        Player *p = qobject_cast<Player*>(c);
        if(p) {
            p->addLife();
        }
    }
}

void GameModeClassic::spawnNewBurwor() {
    Burwor *bw = new Burwor(this, field, samples);
    setupNewEnemy(bw);
    burwors++;
}

void GameModeClassic::spawnNewGarwor() {
    Garwor *bw = new Garwor(this, field, samples);
    setupNewEnemy(bw);
    garwors++;
}

void GameModeClassic::spawnNewThorwor() {
    Thorwor *tw = new Thorwor(this, field, samples);
    setupNewEnemy(tw);
    thorwors++;
}

void GameModeClassic::spawnWorluk() {
    Q_ASSERT(enemyCharacters().isEmpty());
    worluk = new Worluk(this, field, samples);
    setupNewEnemy(worluk);
    connect(worluk, SIGNAL(enteredWarp()), this, SLOT(worlukEscaped()));
    field->setMode(true);
    field->setMapName("WORLUK");
    samples->stopbackground();
    samples->background(11);
}

void GameModeClassic::spawnWizard() {
    Q_ASSERT(enemyCharacters().isEmpty());
    wizard = new Wizard(this, field, samples);
    setupNewEnemy(wizard);
    field->setMode(false);
    field->setMapName("WIZARD OF WOR");
    samples->stopbackground();
    setPlayersControllable(true);
}

void GameModeClassic::setupNewEnemy(EnemyCharacter *c) {
    connect(c, SIGNAL(killed()), this, SLOT(characterKilled()));
    c->resetCharacter();
    c->setPos(field->randomTile(true)->pos());
    c->setVisible(true);
    c->setControllable(true);
    field->scene()->addItem(c);
    characters.append(c);
    c->setDirection(QPoint(-1,0));
    c->setSpeedScale(1.0f+((float)gameRound)/4.0f);
    monstersSpawned++;
}

void GameModeClassic::worlukEscaped() {
    Q_ASSERT(enemyCharacters().size()==1 && worluk);
    Q_ASSERT(characters.removeOne(worluk));
    worluk->deleteLater();
    worluk = 0;
    field->setMode(false);
    field->setMapName("ESCAPED");
    samples->stopbackground();
    setGameState(GS_Intro);
}

void GameModeClassic::playerKilled() {
    if(gameState == GS_Worluk || gameState== GS_Wizard) {
        if(worluk) {
            characters.removeOne(worluk);
            worluk->deleteLater();
            worluk = 0;
            field->setMode(false);
        }
        if(wizard) {
            characters.removeOne(wizard);
            wizard->deleteLater();
            wizard = 0;
        }
        samples->stopbackground();
        setGameState(GS_Intro);
    }
}

void GameModeClassic::worlukKilledBlink() {
    static int blinkCount = 0;
    blinkCount++;
    if(blinkCount%2== 0) {
        field->setBackgroundColor(C64Palette::color(2));
    } else {
        field->setBackgroundColor(C64Palette::color(6));
    }
    if(blinkCount==20) {
        blinkCount = 0;
        worlukKilledTimer.stop();
        field->setBackgroundColor(C64Palette::color(0));
        if(gameRound >= 3) {
            setGameState(GS_Wizard);
        } else {
            setGameState(GS_Intro);
        }
    }
}

void GameModeClassic::wizardKilledBlink() {
    wizardKilledTimer.stop();
    setGameState(GS_Intro);
    nextIntroPhase();
}

void GameModeClassic::setGameState(GameState newState) {
    GameState oldState = gameState;
    gameState = newState;
    if(gameState==GS_Intro) {
        qDebug() << Q_FUNC_INFO << "Intro";
        Q_ASSERT(enemyCharacters().isEmpty());
        nextIntroPhase();
    } else if(gameState==GS_Normal) {
        Q_ASSERT(oldState == GS_Intro);
        Q_ASSERT(enemyCharacters().isEmpty());
        qDebug() << Q_FUNC_INFO << "Normal";
        if(gameRound==3 || gameRound == 12 || gameRound == 19 || gameRound == 27 || gameRound == 39)
            awardBonusPlayer();
        monstersSpawned = burwors = garwors = thorwors = 0;
        worlukKilled = false;
        for(int i=0;i<ENEMY_COUNT;i++)
            spawnNewBurwor();
        setPlayersControllable(true);
    } else if(gameState==GS_Worluk) {
        qDebug() << Q_FUNC_INFO << "Worluk";
        Q_ASSERT(enemyCharacters().isEmpty());
        worlukKilled = false;
        spawnWorluk();
    } else if(gameState==GS_WorlukDeath) {
        qDebug() << Q_FUNC_INFO << "WorlukDeath";
        setPlayersControllable(false);
    } else if(gameState==GS_Wizard) {
        qDebug() << Q_FUNC_INFO << "Wizard";
        Q_ASSERT(enemyCharacters().isEmpty());
        spawnWizard();
    } else if(gameState==GS_WizardDeath) {
        qDebug() << Q_FUNC_INFO << "WizardDeath";
        setPlayersControllable(false);
    } else {
        Q_ASSERT(false);
    }
}
