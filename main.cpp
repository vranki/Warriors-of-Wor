#include <QtWidgets/QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QStringList>
#include "mainwindow.h"

static void setGameDataDirectory()
{
    const QString applicationDir = QCoreApplication::applicationDirPath();
    QStringList candidates;
    candidates << QDir::currentPath();
    candidates << applicationDir;
    candidates << applicationDir + "/..";
    candidates << applicationDir + "/../Resources";

    foreach(const QString &candidate, candidates) {
        QDir dir(candidate);
        if(dir.exists("maps_wow.txt") && QDir::setCurrent(dir.absolutePath())) {
            qDebug() << "Using game data directory" << QDir::currentPath();
            return;
        }
    }

    qWarning() << "Unable to locate game data from" << QDir::currentPath();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    setGameDataDirectory();
    MainWindow w;
    return a.exec();
}
