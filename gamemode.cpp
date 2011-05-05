#include "gamemode.h"

GameMode::GameMode(QObject *parent, QList<Character*> &plrs) :
    QObject(parent), characters(plrs)
{
    field = 0;
    samples = 0;
}
