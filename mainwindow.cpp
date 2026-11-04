#include "mainwindow.h"
#include "ui_mainwindow.h"

int playerColors[8] = {6, 7, 8, 0xF, 3, 2, 0xD, 9};

MainWindow::MainWindow(QWidget *parent) : QGraphicsView(parent), samples(), field(this, characters) {
    QFontDatabase fontdb;
    fontdb.addApplicationFont("Adore64.ttf");
    gameMode = 0;
    setScene(field.scene());
    connect(&mainTimer, SIGNAL(timeout()), this, SLOT(loopTimeout()));
    mainTimer.setInterval(1000 * 1.0/60.0);
    mainTimer.start();
    time.start();
#ifdef HAVE_CWIID
    connect(&wmFinder, SIGNAL(wiimoteFound(WiiMote*)), this, SLOT(wiimoteFound(WiiMote*)));
    wmFinder.scanMote();
#endif
    playerSelectionMenu = 0;
    gameSelectionMenu = 0;
    roundEndScreen = 0;
    gameOverScreen = 0;
    playerSelectionMenu = new PlayerSelectionMenu(characters);
    scene()->addItem(playerSelectionMenu);
    field.setVisible(false);
    connect(this, SIGNAL(playerFound(Player*)), playerSelectionMenu, SLOT(repaintMenu()));
    gameState = GS_PLAYER_SELECT;
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing);
    setGeometry(0,0,640, 400);
    setCursor(Qt::BlankCursor);
    setFocus();
    show();
    showFullScreen();
    grabKeyboard();
}

MainWindow::~MainWindow() {
    resetPlayers();
    if(gameMode)
        gameMode->deleteLater();
}

void MainWindow::loopTimeout() {
    double dt = time.elapsed() / 1000.0;
    if (dt < 0) {
        fprintf(stderr,"MaiWindow::loopTimeout(): dt < 0\n");
        abort();
    }
    time.restart();
    if(dt > 100) dt = 100; // Limit speed on really slow FPS
    for(auto *p : characters) p->tick(dt);
    field.tick(dt);
#ifdef HAVE_CWIID
    wmFinder.pollMotes();
#endif
}

void MainWindow::keyPressEvent (QKeyEvent * e) {
    if(e->isAutoRepeat()) return;
    if(handleLetterKey(e, true)) return;
    QPoint oldDir = arrowKeysDirection;
    if(e->key()==Qt::Key_Left) {
        arrowKeysDirection=QPoint(-1,0);
        e->accept();
    } else if(e->key()==Qt::Key_Right) {
        arrowKeysDirection=QPoint(1,0);
        e->accept();
    } else if(e->key()==Qt::Key_Up) {
        arrowKeysDirection=QPoint(0,-1);
        e->accept();
    } else if(e->key()==Qt::Key_Down) {
        arrowKeysDirection=QPoint(0,1);
        e->accept();
    } else if(e->key()==Qt::Key_Space) {
        emit buttonPressed(1);
        e->accept();
    } else if(e->key()==Qt::Key_R) {
        emit buttonPressed(8);
        wiimoteButtonPressed(8);
        e->accept();
    } else if(e->key()==Qt::Key_S) {
        emit buttonPressed(4);
        wiimoteButtonPressed(4);
        e->accept();
    } else if(e->key()==Qt::Key_Q || e->key()==Qt::Key_Escape) {
        stopGame();
        QCoreApplication::quit();
        e->accept();
    }  else if(e->key()==Qt::Key_F) {
        if(isFullScreen()) {
            showNormal();
        } else {
            showFullScreen();
        }
        e->accept();
    } else if(e->key()==Qt::Key_K && gameState == GS_PLAYER_SELECT && keyboardPlayers.size()<2 && characters.size()<8) {
        Player *p = new Player(this,
                               &field,
                               &samples,
                               C64Palette::color(playerColors[characters.size()]),
                               0,keyboardPlayers.size(),
                               characters.size());
        if(keyboardPlayers.size() == 1) {
            // With two keyboards, the first (blue) player uses the letter keys.
            Player *left = keyboardPlayers.first();
            disconnect(this, SIGNAL(controlInput(QPoint)), left, SLOT(setDirection(QPoint)));
            disconnect(this, SIGNAL(buttonPressed(int)), left, SLOT(fire(int)));
            left->setDirection(QPoint());
            connect(this, SIGNAL(letterControlInput(QPoint)), left, SLOT(setDirection(QPoint)));
            connect(this, SIGNAL(letterButtonPressed(int)), left, SLOT(fire(int)));
        }
        setupPlayer(p, this);
        keyboardPlayers.append(p);
        e->accept();
    }

    if(arrowKeysDirection != oldDir)
        emit controlInput(arrowKeysDirection);
    if(!e->isAccepted())
        e->ignore();
}

bool MainWindow::handleLetterKey(QKeyEvent *event, bool pressed) {
    if(keyboardPlayers.size() != 2 || (pressed && gameState != GS_RUNNING))
        return false;

    QPoint dir;
    switch(event->key()) {
    case Qt::Key_A: dir = QPoint(-1, 0); break;
    case Qt::Key_D: dir = QPoint(1, 0); break;
    case Qt::Key_W: dir = QPoint(0, -1); break;
    case Qt::Key_S: dir = QPoint(0, 1); break;
    case Qt::Key_Control:
        if(pressed) emit letterButtonPressed(1);
        event->accept();
        return true;
    default:
        return false;
    }

    QPoint oldDir = wasdKeysDirection;
    if(pressed)
        wasdKeysDirection = dir;
    else if(wasdKeysDirection == dir)
        wasdKeysDirection = QPoint();
    if(wasdKeysDirection != oldDir)
        emit letterControlInput(wasdKeysDirection);
    event->accept();
    return true;
}

void MainWindow::setupPlayer(Player* p, QObject *controller) {
    p->resetCharacter();

    characters.append(p);

    field.scene()->addItem(p);
    connect(controller, SIGNAL(controlInput(QPoint)), p, SLOT(setDirection(QPoint)));
    connect(controller, SIGNAL(buttonPressed(int)), p, SLOT(fire(int)));
    emit playerFound(p);
    samples.spawn();
}

void MainWindow::stopGame() {
#ifdef HAVE_CWIID
    wmFinder.stopScan();
    wmFinder.quit();
#endif
}

void MainWindow::keyReleaseEvent (QKeyEvent * e) {
    if(e->isAutoRepeat()) return;
    if(handleLetterKey(e, false)) return;
    QPoint oldDir = arrowKeysDirection;
    if(e->key()==Qt::Key_Left || e->key()==Qt::Key_Right) {
        arrowKeysDirection.setX(0);
        e->accept();
    } else if(e->key()==Qt::Key_Up || e->key()==Qt::Key_Down) {
        arrowKeysDirection.setY(0);
        e->accept();
    }

    if(arrowKeysDirection != oldDir)
        emit controlInput(arrowKeysDirection);
    if(!e->isAccepted())
        e->ignore();
}

#ifdef HAVE_CWIID
void MainWindow::wiimoteFound(WiiMote *wm) {
    if(!wm) {
        return;
    }
    Player *p = new Player(this,
                           &field,
                           &samples,
                           C64Palette::color(playerColors[characters.size()]),
                           1,wm->number(),
                           characters.size());
    setupPlayer(p, wm);
    connect(wm, SIGNAL(buttonPressed(int)), this, SLOT(wiimoteButtonPressed(int)));
}
#endif

void MainWindow::resizeEvent(QResizeEvent *event) {
    fitInView(-32,-36,352,236, Qt::KeepAspectRatio);
}

void MainWindow::wiimoteButtonPressed(int buttons) {
    if(gameState==GS_PLAYER_SELECT) {
        if(buttons == 4 && characters.size() > 0) {
            scene()->removeItem(playerSelectionMenu);
            delete playerSelectionMenu;
            playerSelectionMenu = 0;
#ifdef HAVE_CWIID
            wmFinder.stopScan();
#endif
            gameState = GS_GAME_SELECT; // Enter game select
            gameSelectionMenu = new GameSelectionMenu(this, characters);
            connect(gameSelectionMenu, SIGNAL(gameModeSelected(GameMode*)), this, SLOT(gameModeSelected(GameMode*)));
            connect(this, SIGNAL(controlInput(QPoint)), gameSelectionMenu, SLOT(controlInput(QPoint)));
            scene()->addItem(gameSelectionMenu);
            foreach(Character *p, characters)
                p->setVisible(false);
        } else if(buttons == 8) {
            resetPlayers();
            playerSelectionMenu->repaintMenu();
        }
    } else if(gameState==GS_GAME_SELECT) {
        gameSelectionMenu->buttonPressed(buttons);
    } else if(gameState==GS_ROUND_END) {
        if(buttons == 4) {
            scene()->removeItem(roundEndScreen);
            delete roundEndScreen;
            roundEndScreen = 0;
            gameMode->startRound();
        }
    } else if(gameState==GS_GAME_END) {
        if(buttons == 4) {
            scene()->removeItem(gameOverScreen);
            delete gameOverScreen;
            gameOverScreen = 0;
            playerSelectionMenu = new PlayerSelectionMenu(characters);
            scene()->addItem(playerSelectionMenu);
            gameState = GS_PLAYER_SELECT;
        }
    }
}

void MainWindow::resetPlayers() {
    if(gameState != 0) return;
#ifdef HAVE_CWIID
    while(!wmFinder.wiimotes.isEmpty()) {
        WiiMote *m = wmFinder.wiimotes.first();
        wmFinder.freeMote(m);
    }
#endif
    while(!characters.isEmpty()) {
        Character *c= characters.first();
        Player *p = qobject_cast<Player*>(c);
        if(p) {
            SpawnTile *spawn = field.spawnPoint(p->number());
            if(spawn)
                spawn->setUsedByPlayer(false);
        }
        characters.removeOne(c);
        c->deleteLater();
    }
    keyboardPlayers.clear();
    arrowKeysDirection = QPoint();
    wasdKeysDirection = QPoint();
}


void MainWindow::gameModeSelected(GameMode *gm) {
    scene()->removeItem(gameSelectionMenu);
    delete gameSelectionMenu;
    gameSelectionMenu = 0;
    gameMode = gm;
    gameMode->initGame(&field, &samples);
    gameState = GS_RUNNING;
    gameMode->startGame();
    connect(gm, SIGNAL(showRoundEnd(Player*)), this, SLOT(showRoundEnd(Player*)));
    connect(gm, SIGNAL(showGameEnd(Player*)), this, SLOT(showGameEnd(Player*)));
}

void MainWindow::showRoundEnd(Player *winner) {
    field.setVisible(false);
    gameState = GS_ROUND_END;
    roundEndScreen = new RoundEndScreen(characters, winner);
    scene()->addItem(roundEndScreen);
    samples.score();
}

void MainWindow::showGameEnd(Player *winner) {
    field.setVisible(false);

    foreach(Character *c, characters) {
        Player *p = qobject_cast<Player*>(c);
        if(p) {
            p->setDirection(QPoint());
            p->setVisible(false);
            p->setControllable(false);
        }
    }
    gameOverScreen = new GameOverScreen(characters, winner);
    scene()->addItem(gameOverScreen);
    gameState = GS_GAME_END;
    samples.introLong2();
    gameMode->deleteLater();
    gameMode = 0;
}
