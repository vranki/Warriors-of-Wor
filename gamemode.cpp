#include "gamemode.h"

GameMode::GameMode(QObject *parent, QList<Character*> &plrs) :
    QObject(parent), characters(plrs)
{
    field = 0;
    samples = 0;
}

void GameMode::setPlayersControllable(bool controllable) {
    qDebug() << Q_FUNC_INFO << controllable;
    foreach(Character *c, characters) {
        Player *p = qobject_cast<Player*>(c);
        if(p) {
            p->setControllable(controllable);
        }
    }
}
