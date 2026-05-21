#ifndef WIIMOTE_H
#define WIIMOTE_H

#include <QObject>
#include <QDebug>
#include <QPoint>
#include <QTimer>
#ifndef WOW_NO_CWIID
#include <cwiid.h>
#endif

#ifdef WOW_NO_CWIID
typedef void cwiid_wiimote_t;
#endif

class WiiMote : public QObject
{
    Q_OBJECT
public:
    WiiMote(cwiid_wiimote_t * h, int number);
    ~WiiMote();
    int id();
    int number();
    void pollMote();
    static int defaultFlags();
    void close();
    void shake();
public slots:
    void stopShaking();
signals:
    void controlInput(QPoint dir);
    void buttonPressed(int button);
private:
    void setNumber(int num);
    cwiid_wiimote_t *handle;
    QPoint currentDir;
    int currentButtons;
    QTimer rumbleTimer;
    int myNumber;
};

#endif // WIIMOTE_H
