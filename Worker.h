#pragma once

#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtCore/QVector>

#include "Player.h"
#include "gameMap.h"
#include "utils.h"

class Worker : public QThread {
Q_OBJECT

public:
    Worker(const int &id, GameMap &gameMap, QMap<int, Player *> &players, const int &direction, const int &depth, const int &interval, const QPair<int, int> &position);

    ~Worker();

protected:
    void run() override;

private:
    Utils utils;

    QPair<int, int> position;

    QTimer *qTimer;

    QMap<int, Worker *> workers;

    GameMap *gameMap;

    QMap<int, Player *> players;

    QMap<int, int> score;

    QString currentBestMove;

    void testPaths(const int &direction, int &depth, const int &rootDirection);

    QString direction;

    QMap<QString, int> stringToIntDir;

    QMap<QString, QString> reverseDirection;

    int workerCount, id, currentDepth;

    int decideBestWay();

public slots:
    void shutDownWorker();

signals:
    void resultReady(const int &id, const int &depth);
};
