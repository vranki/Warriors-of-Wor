#include "wiimotefinder.h"

WiimoteFinder::WiimoteFinder() : QThread()
{
    foundWiimote = 0;
    foundWiimoteReally = false;
    foundWiimoteHandle = 0;
    scanning = false;
}

WiimoteFinder::~WiimoteFinder()
{
    if(isRunning())
        quit();
    qDeleteAll(wiimotes);
}

void WiimoteFinder::scanMote() {
    scanning = true;
    if(!isRunning()) start();
}

void WiimoteFinder::stopScan() {
    scanning = false;
    quit();
}

void WiimoteFinder::pollMotes() {
    if(foundWiimoteReally) {
        if(foundWiimoteHandle) {
            foundWiimote = new WiiMote(foundWiimoteHandle, wiimotes.size());
            wiimotes.append(foundWiimote);
        }
        foundWiimoteReally = false;
        foundWiimoteHandle = 0;
        emit wiimoteFound(foundWiimote);
        foundWiimote = 0;
        if(!isRunning() && scanning)
            start();
    }
    foreach(WiiMote *wm, wiimotes) {
        wm->pollMote();
    }
    foreach(WiiMote *wm, freeMotes) {
        wm->pollMote();
    }
}

void WiimoteFinder::run() {
//    qDebug() << Q_FUNC_INFO;
    bdaddr_t bdaddr = {{0,0,0,0,0,0}};
    memset(&bdaddr,0,sizeof(bdaddr_t));
    foundWiimoteHandle = cwiid_open(&bdaddr, WiiMote::defaultFlags());
    foundWiimoteReally = true;
}

void WiimoteFinder::freeMote(WiiMote *wm) {
    qDebug() << Q_FUNC_INFO << wm;
    freeMotes.append(wm);
    Q_ASSERT(wiimotes.removeOne(wm));
    wm->disconnect();
    connect(wm, SIGNAL(buttonPressed(int)), this, SLOT(freeMoteWantsToJoin(int)));
}

void WiimoteFinder::freeMoteWantsToJoin(int buttons) {
    if(!buttons) return;
    WiiMote *wm = qobject_cast<WiiMote*> (sender());
    Q_ASSERT(wm);
    Q_ASSERT(freeMotes.removeOne(wm));
    qDebug() << Q_FUNC_INFO << wm;
    disconnect(wm, SIGNAL(buttonPressed(int)), this, SLOT(freeMoteWantsToJoin(int)));
    wiimotes.append(wm);
    wm->shake();
    emit wiimoteFound(wm);
}
