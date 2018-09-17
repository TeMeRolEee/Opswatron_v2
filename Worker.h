#pragma once

#include <QtCore/QThread>
#include "Player.h"
#include "gameMap.h"

class Worker: public QThread {
Q_OBJECT

public:
    Worker(const int &id, GameMap &gameMap, QMap<int, Player*> &players, const int &direction);
    ~Worker();

protected:
    void run() override;

private:
    GameMap *gameMap;

    QMap<int, Player*> players;

    Player me;

    int id;

    bool isRunning;

    QString findPath();

    QString currentBestMove;

    QString direction;

public slots:
    void shutDownWorker();

signals:
    void resultReady(const int &id ,const QString &qString);
};
