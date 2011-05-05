#include "explosion.h"

Explosion::Explosion(QObject *parent) : Sprite(parent) {
    frame1 = loadBitmap("wow-sprites/spr251E.bmp", palette.color(7));
    frame2 = loadBitmap("wow-sprites/spr255E.bmp", palette.color(7));
    setPixmap(frame1);
    setOffset(2,2);
    QTimer::singleShot(100, this, SLOT(swapFrame()));
    frameNum = 0;
}

void Explosion::swapFrame() {
    frameNum++;
    if(frameNum % 2) {
        setPixmap(frame2);
    } else {
        setPixmap(frame1);
    }
    if(frameNum > 8) {
        scene()->removeItem(this);
        deleteLater();
    } else {
        QTimer::singleShot(150, this, SLOT(swapFrame()));
    }
}
