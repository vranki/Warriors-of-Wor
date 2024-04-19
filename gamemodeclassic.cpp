#include "gamemodeclassic.h"

GameModeClassic::GameModeClassic(QObject *parent, QList<Character*> &plrs) : GameMode(parent, plrs) {
    qDebug() << Q_FUNC_INFO;
    gameRound = 0;
    gameState = GS_Intro;
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
    qDebug() << Q_FUNC_INFO;
    killAllEnemies();
}

void GameModeClassic::killAllEnemies() {
    qDebug() << Q_FUNC_INFO;
    foreach(EnemyCharacter *c, enemyCharacters()) {
        characters.removeOne(c);
        c->deleteLater();
    }
}

void GameModeClassic::initGame(Playfield *f, SamplePlayer *s) {
    qDebug() << Q_FUNC_INFO;
    field = f;
    samples = s;
    field->loadMapData("maps_wow.txt");
}

QString GameModeClassic::name() const {
    return "CLASSIC";
}

QString GameModeClassic::description() const {
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
        QPixmap pixmap("wow-sprites/getready.png");
        getready = new QGraphicsPixmapItem(pixmap);
        field->scene()->addItem(getready);
        getready->setPixmap(pixmap) ;
        getready->setZValue(100);
        samples->stopbackground();
        samples->introLong2();
        introTimer.start(4300);
    } else if(introPhase==2) { // Go
        Q_ASSERT(getready);
        if(doubleScoreDungeon) {
            if (getready) delete getready;
            getready = nullptr;
            Q_ASSERT(!dsd);
            QPixmap pixmap("wow-sprites/doublescoredungeon.png");
            dsd = new QGraphicsPixmapItem(pixmap);
            field->scene()->addItem(dsd);
            dsd->setPixmap(pixmap) ;
            dsd->setZValue(100);
            field->setVisible(false);
            foreach(Character *c, characters)
                c->setVisible(false);
        } else {
            Q_ASSERT(!go);
            QPixmap pixmap("wow-sprites/go.png");
            go = new QGraphicsPixmapItem(pixmap);
            field->scene()->addItem(go);
            go->setZValue(100);
        }
        if(bonusPlayerGiven) {
            Q_ASSERT(!bonusPlayer);
            QPixmap pixmap("wow-sprites/bonus_player.png");
            bonusPlayer = new QGraphicsPixmapItem(pixmap);
            bonusPlayer->setZValue(100);
            field->scene()->addItem(bonusPlayer);
            bonusPlayerGiven = false;
        }
        introTimer.start(4000);
    } else if(introPhase==3) {
        if(getready) {
            delete getready;
            getready = nullptr;
        }
        if(go) {
            delete go;
            go = nullptr;
        }
        if(dsd) {
            delete dsd;
            dsd = nullptr;
        }
        if(bonusPlayer) {
            delete bonusPlayer;
            bonusPlayer = nullptr;
        }
        field->setVisible(true);
        foreach(Character *c, characters)
            c->setVisible(true);
        getready = go = dsd = bonusPlayer = nullptr;
        introPhase = 0;
        startRound();
    }
}

void GameModeClassic::startRound() {
    wizard = 0;
    worluk = 0;
    int newMapToLoad = gameRound;
    qDebug() << Q_FUNC_INFO << "gameRound:" << gameRound;
    if (field->getMapCount()) {
	// data not available prior to reading maps
        //qDebug() << Q_FUNC_INFO << "field->getMapCount():" << field->getMapCount();
        newMapToLoad = gameRound % field->getMapCount();
    }
    //qDebug() << "Computed newMapToLoad as " << newMapToLoad;
    field->loadMap( newMapToLoad );
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
    //qDebug() << Q_FUNC_INFO << "characters.size(): " << characters.size();
    Character *c = qobject_cast<Character*>(sender());
    Q_ASSERT(c);
    Burwor *burwor = qobject_cast<Burwor*>(sender());
    Garwor *garwor = qobject_cast<Garwor*>(sender());
    Thorwor *thorwor = qobject_cast<Thorwor*>(sender());

    Q_ASSERT(characters.contains(c));
    characters.removeOne(c);
    //qDebug() << "characterssize(): " << characters.size();
    c->deleteLater();
    c = 0;
    QList<EnemyCharacter*> enemies = enemyCharacters();
    if(burwor) {
        //qDebug() << Q_FUNC_INFO << "Detected death of burwor";
        if(garwors < gameRound && enemies.size()<gameRound)
            spawnNewGarwor();
    } else if(garwor) {
        //qDebug() << Q_FUNC_INFO << "Detected death of garwor";
        spawnNewThorwor();
    } else if(worluk) {
        qDebug() << Q_FUNC_INFO << "Detected death of worluk";
        samples->stopbackground();
        worlukKilled = true;
        worluk = 0;
        setGameState(GS_WorlukDeath);
        worlukKilledTimer.start();
        field->setMode(1);
    } else if(wizard) {
        //qDebug() << Q_FUNC_INFO << "Detected death of wizard";
        wizard = 0;
        field->setMode(2);
        wizardKilledTimer.start();
        setGameState(GS_WizardDeath);
    }
    if(enemyCharacters().isEmpty() && gameState == GS_Normal) {
#define SPAWN_WORLUK_AND_WIZARD
#ifdef SPAWN_WORLUK_AND_WIZARD
        qDebug() << Q_FUNC_INFO << "No more enemies, triggering worluk";
        setGameState(GS_Worluk);
#else
        qDebug() << Q_FUNC_INFO << "No more enemies, configured to skip worluk/wizard";
        setGameState(GS_Intro);
#endif
    }
}

QList<EnemyCharacter*> GameModeClassic::enemyCharacters() {
    //qDebug() << Q_FUNC_INFO << "characters.size():" << characters.size();
    QList<EnemyCharacter*> enemies;
    foreach(Character *c, characters) {
        EnemyCharacter *ec = qobject_cast<EnemyCharacter*>(c);
        if(ec) {
            enemies.append(ec);
        }
    }
    return enemies;
}



void GameModeClassic::awardBonusPlayer() {
    //qDebug() << Q_FUNC_INFO;
    bonusPlayerGiven = true;
    foreach(Character *c, characters) {
        Player *p = qobject_cast<Player*>(c);
        if(p) {
            p->addLife();
        }
    }
}

void GameModeClassic::spawnNewBurwor() {
    //qDebug() << Q_FUNC_INFO << "characters.size(): " << characters.size();
    Burwor *bw = new Burwor(this, field, samples);
    if (!bw) {
        qDebug() << "Could not create Burwor.";
        abort();
    }
    setupNewEnemy(bw);
    burwors++;
}

void GameModeClassic::spawnNewGarwor() {
    //qDebug() << Q_FUNC_INFO << "characters.size(): " << characters.size();
    Garwor *gw = new Garwor(this, field, samples);
    if (!gw) {
        qDebug() << "Could not create Garwor.";
        abort();
    }
    setupNewEnemy(gw);
    garwors++;
}

void GameModeClassic::spawnNewThorwor() {
    //qDebug() << Q_FUNC_INFO << "characters.size(): " << characters.size();
    Thorwor *tw = new Thorwor(this, field, samples);
    if (!tw) {
        qDebug() << "Could not create Thorwor.";
        abort();
    }
    setupNewEnemy(tw);
    thorwors++;
}

void GameModeClassic::spawnWorluk() {
    qDebug() << Q_FUNC_INFO << "characters.size(): " << characters.size();
    Q_ASSERT(enemyCharacters().isEmpty());
    worluk = new Worluk(this, field, samples);
    if (!worluk) {
        qDebug() << "Could not create worluk.";
        abort();
    }
    setupNewEnemy(worluk);
    connect(worluk, SIGNAL(enteredWarp()), this, SLOT(worlukEscaped()));
    field->setMode(true);
    field->setMapName("WORLUK");
    samples->stopbackground();
    samples->background(GS_BACKGROUND_WORLUK);
}

void GameModeClassic::spawnWizard() {
    qDebug() << Q_FUNC_INFO << "characters.size(): " << characters.size();
    Q_ASSERT(enemyCharacters().isEmpty());
    wizard = new Wizard(this, field, samples);
    if (!wizard) {
        qDebug() << "Could not create wizard.";
        abort();
    }
    setupNewEnemy(wizard);
    field->setMode(false);
    field->setMapName("WIZARD OF WOR");
    samples->stopbackground();
    setPlayersControllable(true);
}

void GameModeClassic::setupNewEnemy(EnemyCharacter *c) {
    //qDebug() << Q_FUNC_INFO << "characters.size(): " << characters.size();
    if (!c) {
        qDebug() << "setupNewEnemy EnemyCharacter is NULL";
        return;
    }
    //qDebug() << "setupNewEnemy: connecting signal";
    connect(c, SIGNAL(killed()), this, SLOT(characterKilled()));
    //qDebug() << "setupNewEnemy: connected";
    //qDebug() << "characters.size(): " << characters.size();
    c->resetCharacter();
    //qDebug() << "characters.size(): " << characters.size();
    //qDebug() << "setupNewEnemy: setting position";
    MapTile *mt = field->randomTile(true);
    c->setPos(mt->pos());
    //qDebug() << "setupNewEnemy: let it be visible";
    c->setVisible(true);
    //qDebug() << "setupNewEnemy: and it shall be controllable";
    c->setControllable(true);
    //qDebug() << "setupNewEnemy: adding item to scene";
    field->scene()->addItem(c);
    //qDebug() << "characters.size(): " << characters.size();
    characters.append(c);
    c->setDirection(QPoint(-1,0));
    c->setSpeedScale(1.0f+((float)gameRound)/4.0f);
    monstersSpawned++;
    //qDebug() << Q_FUNC_INFO << "done, now with " << monstersSpawned << " monsters.";
}

void GameModeClassic::worlukEscaped() {
    qDebug() << Q_FUNC_INFO << "worlukEscaped";
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
    qDebug() << Q_FUNC_INFO << " with gameState " << gameState;
    if(gameState == GS_Worluk || gameState== GS_Wizard) {
        if(worluk) {
            characters.removeOne(worluk);
            worluk->deleteLater();
            worluk = nullptr;
            field->setMode(false);
        }
        if(wizard) {
            characters.removeOne(wizard);
            wizard->deleteLater();
            wizard = nullptr;
        }
        samples->stopbackground();
        setGameState(GS_Intro);
    }
}

void GameModeClassic::worlukKilledBlink() {
    qDebug() << Q_FUNC_INFO ;
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
    qDebug() << Q_FUNC_INFO ;
    wizardKilledTimer.stop();
    setGameState(GS_Intro);
    nextIntroPhase();
}

void GameModeClassic::setGameState(GameState newState) {
    GameState oldState = gameState;
    qDebug() << Q_FUNC_INFO << "oldState:" << oldState << " -> newState:" << newState << " for gameRound:" << gameRound;
    gameState = newState;
    if(gameState==GS_Intro) {
        //qDebug() << Q_FUNC_INFO << "Intro";
        Q_ASSERT(enemyCharacters().isEmpty());
        nextIntroPhase();
    } else if(gameState==GS_Normal) {
        Q_ASSERT(oldState == GS_Intro);
        Q_ASSERT(enemyCharacters().isEmpty());
        //qDebug() << Q_FUNC_INFO << "Normal";
        if( gameRound==3 || gameRound == 12 || gameRound == 19 || gameRound == 27 || gameRound == 39) {
            awardBonusPlayer();
        }
        monstersSpawned = burwors = garwors = thorwors = 0;
        worlukKilled = false;
        //qDebug() << "About to spawn " << ENEMY_COUNT << " new enemies.";
        for(int i=0;i<ENEMY_COUNT;i++)
            spawnNewBurwor();
        //qDebug() << "All new enemies created.";
        setPlayersControllable(true);
    } else if(gameState==GS_Worluk) {
        //qDebug() << Q_FUNC_INFO << "Worluk";
        Q_ASSERT(enemyCharacters().isEmpty());
        worlukKilled = false;
        spawnWorluk();
    } else if(gameState==GS_WorlukDeath) {
        //qDebug() << Q_FUNC_INFO << "WorlukDeath";
        setPlayersControllable(false);
    } else if(gameState==GS_Wizard) {
        //qDebug() << Q_FUNC_INFO << "Wizard";
        Q_ASSERT(enemyCharacters().isEmpty());
        spawnWizard();
    } else if(gameState==GS_WizardDeath) {
        //qDebug() << Q_FUNC_INFO << "WizardDeath";
        setPlayersControllable(false);
    } else {
        Q_ASSERT(false);
    }
}
