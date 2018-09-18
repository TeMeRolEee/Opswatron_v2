#pragma once

#include <QtCore/QThread>
#include <QtCore/QTimer>
#include "Player.h"
#include "gameMap.h"

class Worker : public QThread {
Q_OBJECT

public:
    Worker(const int &id, GameMap &gameMap, QMap<int, Player *> &players, const int &direction, const int &depth, const int &interval);

    ~Worker();

protected:
    void run() override;

private:

    QTimer *qTimer;

    QMap<int, Worker *> workers;

    Player me;

    GameMap *gameMap;

    QMap<int, Player *> players;

    void createSubWorkers(const QVector<int> &directions);

    bool checkObstacle(const QPair<int, int> &position, const QString &direction);

    QPair<int, int> returnNextPos(const QPair<int, int> &currentPosition, const QString &direction);

    int workerCount, id, currentDepth;

    bool isRunning;

    QString findPath();

    QString currentBestMove;

    QString direction;

    QMap<QString, int> stringToIntDir;

public slots:

    void shutDownWorker();

signals:

    void resultReady(const int &id, const QString &qString);
};
