#ifndef SPRITE_H
#define SPRITE_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include "C64Palette.h"

class Sprite : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Sprite(QObject *parent);
    static QPixmap loadBitmap(QString filename, int colorindex);
    static QPixmap loadBitmap(QString filename, QColor color);
private:

};

#endif // SPRITE_H
