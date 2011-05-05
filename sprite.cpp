#include "sprite.h"

Sprite::Sprite(QObject *parent) : QObject(parent), QGraphicsPixmapItem(){
}


QPixmap Sprite::loadBitmap(QString filename, int colorindex) {
    C64Palette palette;
    return loadBitmap(filename, palette.color(colorindex));
}

QPixmap Sprite::loadBitmap(QString filename, QColor color) {
    QImage image(filename);
    image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    for(int x=0;x < image.width();x++) {
        for(int y=0;y < image.height();y++) {
            QColor pixel(image.pixel(x,y));
            if(pixel.red()==0 && pixel.green()==0 && pixel.blue()==0) {
                pixel.setAlpha(0);
            } else {
                pixel.setRed(color.red());
                pixel.setGreen(color.green());
                pixel.setBlue(color.blue());
            }
            image.setPixel(x, y, pixel.rgba());
        }
    }
    Q_ASSERT(image.hasAlphaChannel());
    QPixmap sprite = QPixmap::fromImage(image);
    //Q_ASSERT(sprite.hasAlphaChannel());
    Q_ASSERT(!sprite.isNull());
    return sprite;
}
