#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <QPixmap>
#include <QImage>
#include <QTimer>
#include <QGraphicsScene>
#include "sprite.h"
#include "C64Palette.h"

class Explosion : public Sprite
{
    Q_OBJECT
public:
    explicit Explosion(QObject *parent = 0);

signals:

public slots:
    void swapFrame();
private:
    QPixmap frame1;
    QPixmap frame2;
    int frameNum;
    C64Palette palette;
};

#endif // EXPLOSION_H
