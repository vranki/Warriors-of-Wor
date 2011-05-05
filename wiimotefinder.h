#ifndef WIIMOTEFINDER_H
#define WIIMOTEFINDER_H

#include <cwiid.h>
#include <QDebug>
#include <QList>
#include <QObject>
#include <QThread>
#include "wiimote.h"

class WiimoteFinder :public QThread
{
    Q_OBJECT
public:
    WiimoteFinder();
    ~WiimoteFinder();
    void scanMote();
    void stopScan();
    void pollMotes();
    QList<WiiMote*> wiimotes;
    void freeMote(WiiMote *wm);
public slots:
    void freeMoteWantsToJoin(int buttons);

protected:
    void run();
signals:
    void wiimoteFound(WiiMote *wm);
private:
    WiiMote *foundWiimote;
    bool foundWiimoteReally;
    cwiid_wiimote_t * foundWiimoteHandle;
    QList<WiiMote*> freeMotes;
    bool scanning;
};

#endif // WIIMOTEFINDER_H
