#ifndef RADAR_H
#define RADAR_H

#include <QGraphicsItem>
#include <QPainter>
#include <C64Palette.h>
#include <QList>
#include <QTimer>
#include "character.h"

#define RADARW 95
#define RADARH 40

class Radar : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit Radar(QObject *parent, QList<Character*> &plrs);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
signals:

public slots:
    void updateMap();
private:
    QList<Character*> &characters;
    QTimer updateTimer;
};

#endif // RADAR_H
