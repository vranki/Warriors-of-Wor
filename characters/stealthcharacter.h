#ifndef STEALTHCHARACTER_H
#define STEALTHCHARACTER_H

#include "enemycharacter.h"
#include <QTimer>

class StealthCharacter : public EnemyCharacter
{
    Q_OBJECT
public:
    explicit StealthCharacter(QObject *parent,
                              PlayfieldInfo *pfinfo,
                              SamplePlayer *smp);

signals:

public slots:
    virtual void tileEntered(MapTile *mt);
private slots:
    void checkIfSeesPlayer();
protected:
    bool seesPlayer();
    bool stealthMode; // AKA invisible
    int stealthProbability; // Less is more
private:
    QTimer checkTimer;
};

#endif // STEALTHCHARACTER_H
