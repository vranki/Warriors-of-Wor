#include "gamemenu.h"

PlayerSelectionMenu::PlayerSelectionMenu(QList<Character*> &plrs) : QObject(), QGraphicsItem(), players(plrs) {
    colorCycle = 0;
}

QRectF PlayerSelectionMenu::boundingRect() const {
    return QRectF(-5,-5,325, 205);
}
void PlayerSelectionMenu::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                QWidget *widget) {
    colorCycle++;
    if(colorCycle==16) colorCycle=1;
    if(isVisible())
        QTimer::singleShot(50, this, SLOT(repaintMenu()));
    QPen pen;
    C64Palette palette;
    pen.setBrush(QBrush(palette.color(colorCycle)));
    QFont font = QFont("Adore64");
    font.setPointSize(6);
    painter->setPen(pen);
    painter->setFont(font);
    //painter->drawRect(5, 5, 315, 195);
    painter->drawText(50,32, "*** Warriors of Wor ***");
    pen.setBrush(QBrush(palette.color(6)));
    painter->setPen(pen);
    painter->drawText(50,20, "carambola pyre presents");
    painter->drawText(20,62, "Press 1+2 on nearest Wiimote to join");
    painter->drawText(20,184, "+ to continue, - to clear players");
    int ypos = 80;
    int xpos = 10;
    foreach(Character *c, players) {
        Player *p = qobject_cast<Player*> (c);
        if(p) {
            if(p->number() == 4)  {
                xpos = 160;
                ypos = 80;
            }
            QString ctrlType = "Keyboard";
            QString ctrlId = "";
            if(p->controllerType() == 1) {
                ctrlType = "Wiimote";
                if(p->controllerId()==0) ctrlId = "[*   ]";
                if(p->controllerId()==1) ctrlId = "[ *  ]";
                if(p->controllerId()==2) ctrlId = "[  * ]";
                if(p->controllerId()==3) ctrlId = "[   *]";
                if(p->controllerId()==4) ctrlId = "[ ***]";
                if(p->controllerId()==5) ctrlId = "[* **]";
                if(p->controllerId()==6) ctrlId = "[** *]";
                if(p->controllerId()==7) ctrlId = "[*** ]";
            }
            painter->drawText(xpos+30,ypos, "Player " +
                              QString::number(players.indexOf(p)+1));
            painter->drawText(xpos+30,ypos+10, ctrlType + " " + ctrlId);
            p->setPos(xpos, ypos-10);
            p->setVisible(true);
            ypos += 24;
        }
    }
}

void PlayerSelectionMenu::repaintMenu() {
    update();
}
