#include "Worker.h"

#include <QDebug>

#include "Core.h"
#include "directions.h"


Worker::Worker(const int &id, GameMap &gameMap, QMap<int, Player *> &players, const int &direction, const int &depth,
               const int &interval, const QPair<int, int> &position) : id(id),
                                                                       gameMap(&gameMap),
                                                                       players(players),
                                                                       direction(direction),
                                                                       currentDepth(depth),
                                                                       position(position) {
    stringToIntDir = {
            {"UP",    0},
            {"DOWN",  1},
            {"LEFT",  2},
            {"RIGHT", 3}
    };
    reverseDirection = {
            {"UP",    "DOWN"},
            {"DOWN",  "UP"},
            {"LEFT",  "RIGHT"},
            {"RIGHT", "LEFT"}
    };
    score = {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 0}
    };
    qTimer->setInterval(interval);
}

Worker::~Worker() {
    delete gameMap;
}

void Worker::run() {
    qTimer->start();
    int result;

    QVector<int> possibleDirections;
    int currentReverseDirection = stringToIntDir.value(reverseDirection.value(direction));

    for (const auto &value : stringToIntDir) {
        if ((value != currentReverseDirection) &&
            (utils.checkObstacle(position, stringToIntDir.key(value), *gameMap))) {
            possibleDirections.push_back(value);
        }
    }
    for (const auto &currentDirection : stringToIntDir.keys()) {
        if (direction != reverseDirection.value(currentDirection)) {
            int dir = stringToIntDir.value(currentDirection);
            testPaths(dir, currentDepth, dir);
        }

    }


    result = decideBestWay();

    qDebug() << QThread::currentThreadId();

    emit resultReady(id, result);
}

void Worker::shutDownWorker() {
    emit resultReady(id, currentDepth);
}


int Worker::decideBestWay() {
    int bestDirection = stringToIntDir.value(direction);
    int bestScore = score.value(bestDirection);

    for (auto currentDir : score.keys()) {
        if (score.value(currentDir) > bestScore) {
            bestScore = score.value(currentDir);
            bestDirection = currentDir;
        }
    }
}

void Worker::testPaths(const int &direction, int &depth, const int &rootDirection) {
    int nextDepth = depth;
    if (nextDepth < 3) {


        for (const auto &currentDirection : stringToIntDir.keys()) {
            if (direction != reverseDirection.value(currentDirection)) {
                QPair<int, int> nextPath = utils.returnNextPos(position, direction);
                if (!utils.checkObstacle(nextPath, currentDirection, *gameMap)) {
                     testPaths(stringToIntDir.value(currentDirection), nextDepth, rootDirection);
                } else {
                    score[rootDirection] = nextDepth;
                }
            }

        }
        nextDepth++;
    }
}

