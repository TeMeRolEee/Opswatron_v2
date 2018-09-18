#include "Worker.h"
#include "Core.h"
#import "directions.h"

#include <QDebug>

Worker::Worker(const int &id, GameMap &gameMap, QMap<int, Player*> &players, const int &direction, const int &depth, const int &interval): id(id),
    gameMap(&gameMap),
    players(players),
    direction(direction),
    currentDepth(depth){
    stringToIntDir= {
            {"UP", 0},
            {"DOWN", 1},
            {"LEFT", 2},
            {"RIGHT", 3}
    };
    qTimer->setInterval(interval);
}

Worker::~Worker() {
    delete gameMap;
}

void Worker::run() {
    qTimer->start();
    QString result;

    qDebug() << QThread::currentThreadId();

    emit resultReady(id, result);
}

QString Worker::findPath() {
    return QString();
}

void Worker::shutDownWorker() {
    emit resultReady(id, currentBestMove);
}

void Worker::createSubWorkers(const QVector<int> &directions) {
    int nextDepth = currentDepth;

    if (currentDepth < 3) {
        nextDepth++;

        for (auto currentDirection : directions) {
            auto *worker = new Worker(workerCount, *gameMap, players, currentDirection, nextDepth,
                                      static_cast<int>(qTimer->interval()*0.65));
            workers.insert(workerCount, worker);
            workerCount++;

            connect(worker, &Worker::resultReady, this, &Core::handleResults);
            connect(this, &Core::getResultNow, worker, &Worker::shutDownWorker);
            connect(qTimer, &QTimer::timeout, worker, &Worker::shutDownWorker);
            worker->start();
        }
    }
}

bool Worker::checkObstacle(const QPair<int, int> &currentPosition, const QString &direction) {
    return gameMap->getPositionInfo(returnNextPos(currentPosition, direction)) == 1;
}

QPair<int, int> Worker::returnNextPos(const QPair<int, int> &currentPosition, const QString &direction) {
    switch (stringToIntDir[direction]) {
        case static_cast<int>(Directions::UP):
            return QPair<int, int>(currentPosition.first-1, currentPosition.second);
        case static_cast<int>(Directions::DOWN):
            return QPair<int, int>(currentPosition.first+1, currentPosition.second);
        case static_cast<int>(Directions::LEFT):
            return QPair<int, int>(currentPosition.first, currentPosition.second-1);
        case static_cast<int>(Directions::RIGHT):
            return QPair<int, int>(currentPosition.first-1, currentPosition.second+1);
        default:break;
    }
}

