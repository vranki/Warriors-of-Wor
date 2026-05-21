#include "sprite.h"
#include <QDebug>
#include <QMap>

Sprite::Sprite(QObject *parent) : QObject(parent), QGraphicsPixmapItem(){
}


QPixmap Sprite::loadBitmap(QString filename, int colorindex) {
    C64Palette palette;
    return loadBitmap(filename, palette.color(colorindex));
}

QPixmap Sprite::loadBitmap(QString filename, QColor color) {
    static QMap<QString, QPixmap> cache;
    const QString cacheKey = filename + "|" + QString::number(color.rgba(), 16);
    if(cache.contains(cacheKey))
        return cache.value(cacheKey);

    QImage image(filename);
    if(image.isNull()) {
        qDebug() << Q_FUNC_INFO << "Unable to load image " << filename;
        image = QImage(24, 24, QImage::Format_ARGB32_Premultiplied);
        image.fill(Qt::transparent);
        for(int x=4; x < 20; x++) {
            for(int y=4; y < 20; y++) {
                image.setPixel(x, y, color.rgba());
            }
        }
    }
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
    QPixmap sprite = QPixmap::fromImage(image);
    if(sprite.isNull()) {
        sprite = QPixmap(24, 24);
        sprite.fill(color);
    }
    cache.insert(cacheKey, sprite);
    return sprite;
}
