#include "gameselectionmenu.h"

GameSelectionMenu::GameSelectionMenu(QObject *parent, QList<Character*> &plrs) : QObject(parent), QGraphicsItem(), characters(plrs) {
    colorCycle = 0;
    currentGameMode = 0;
    gameModeNumber = 0;
    if(!currentGameMode)
        currentGameMode = new GameModeClassic(parent, characters); // NOT this!
}

QRectF GameSelectionMenu::boundingRect() const {
    return QRectF(-5,-5,325, 205);
}

void GameSelectionMenu::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                              QWidget *widget) {
    colorCycle++;
    if(colorCycle==16) colorCycle=1;
    if(isVisible())
        QTimer::singleShot(50, this, SLOT(repaintMenu()));
    QPen pen;
    QFont font = QFont("Adore64");
    font.setPointSize(6);
    pen.setBrush(QBrush(C64Palette::color(6)));
    painter->setPen(pen);
    painter->setFont(font);
    painter->drawText(40,16, "select game mode");
    painter->setPen(pen);
    painter->drawText(50,32, "< " + currentGameMode->name() + " >");
    painter->setPen(pen);
    painter->drawText(20,62, currentGameMode->description());
}

void GameSelectionMenu::buttonPressed(int button) {
    if(button == 4)
        emit gameModeSelected(currentGameMode);
}

void GameSelectionMenu::repaintMenu() {
    update();
}

void GameSelectionMenu::controlInput(QPoint dir) {
    int oldGameMode = gameModeNumber;
    if(dir.x() > 0) {
        gameModeNumber++;
        if(gameModeNumber == GAME_MODE_COUNT)
            gameModeNumber = 0;
    } else if(dir.x() < 0) {
        gameModeNumber--;
        if(gameModeNumber < 0) gameModeNumber = GAME_MODE_COUNT - 1;
    }
    delete currentGameMode;
    if(gameModeNumber==0) {
        currentGameMode = new GameModeClassic(parent(), characters); // NOT this!
    } else if(gameModeNumber==1) {
         currentGameMode = new GameModeDeathmatch(parent(), characters); // NOT this!
    }
}
