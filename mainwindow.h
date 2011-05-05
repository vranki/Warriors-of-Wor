#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QDebug>
#include <QPoint>
#include <QKeyEvent>
#include <QGraphicsView>
#include <QFontDatabase>
#include "playfield.h"
//#include "character.h"
#include "wiimote.h"
#include "wiimotefinder.h"
#include "gamemenu.h"
#include "player.h"
#include "sampleplayer.h"
#include "C64Palette.h"
#include "gameselectionmenu.h"
#include "roundendscreen.h"
#include "gameoverscreen.h"
#include "gamemode.h"


enum gameStates {
    GS_PLAYER_SELECT=0,
    GS_RUNNING,
    GS_GAME_SELECT,
    GS_ROUND_END,
    GS_GAME_END
};

class MainWindow : public QGraphicsView
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void keyPressEvent (QKeyEvent * e);
    void keyReleaseEvent (QKeyEvent * e);
    void startGame();
    void startRound();
    void resetPlayers();
public slots:
    void loopTimeout();
    void wiimoteFound(WiiMote *wm);
    void wiimoteButtonPressed(int buttons);
    void gameModeSelected(GameMode *gm);
    void showRoundEnd(Player *winner);
    void showGameEnd(Player *winner);
signals:
    void controlInput(QPoint dir);
    void buttonPressed(int button);
    void playerFound(Player *player);

protected:
    virtual void resizeEvent(QResizeEvent *event);
private:
    void setupPlayer(Player* p, QObject *controller);
    SamplePlayer samples;
    Playfield field;
    QTimer mainTimer;
    QTime time;
    QPoint controllerDir;
    WiimoteFinder wmFinder;
    PlayerSelectionMenu *playerSelectionMenu;
    GameSelectionMenu *gameSelectionMenu;
    RoundEndScreen *roundEndScreen;
    GameOverScreen *gameOverScreen;
    QList<Character*> characters;
    bool keyboardPlayerCreated;
    gameStates gameState; // 0 = player select, 1= running, 2=game select, 3=round end, 4=game end
    GameMode *gameMode;
};

#endif // MAINWINDOW_H
