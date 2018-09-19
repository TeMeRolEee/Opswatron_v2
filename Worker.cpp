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
    //connect(this, &Worker::resultReady, this, []() {
    //   qDebug() << "MIAFASZOMEZASZARMAAAAAA";
    //});
}

Worker::~Worker() {
}

void Worker::run() {
    //qDebug() << QThread::currentThreadId();
    QVector<int> possibleDirections;
    int currentReverseDirection = stringToIntDir.value(reverseDirection.value(direction));

    for (const auto &value : stringToIntDir) {
        //qDebug() << "run" << (stringToIntDir.key(value) != stringToIntDir.key(currentReverseDirection) &&
        //             !(utils.checkObstacle(position, stringToIntDir.key(value), *gameMap)));
        if (stringToIntDir.key(value) != stringToIntDir.key(currentReverseDirection) &&
            !(utils.checkObstacle(position, stringToIntDir.key(value), *gameMap))) {
            //qDebug() << "starting testPath" << value << currentDepth;
            testPaths(value, currentDepth, 1, position);
        }
    }


    result = utils.decideBestWay(score);
    qDebug() << "worker_score" << id << ":" << score;
    qDebug() << "result:" << id << ":" << result;


    //emit resultReady(id, result.first, result.second);
}
/*
void Worker::shutDownWorker() {
    emit resultReady(id, result.first, result.second);
}*/

void Worker::testPaths(const int &rootDirection, const int &currentDirection, const int &depth, QPair<int, int> &pos) {
    if (depth < 4) {
        qDebug() << depth;
        for (const auto &currentAutoDirection : stringToIntDir.keys()) {
            qDebug() << "yes_or_no"
                     << ((currentAutoDirection != reverseDirection.value(stringToIntDir.key(currentDirection))) &&
                            !(utils.checkObstacle(position, stringToIntDir.key(stringToIntDir.value(currentAutoDirection)),
                                                  *gameMap)));
            if ((currentAutoDirection != reverseDirection.value(stringToIntDir.key(currentDirection))) &&
                !(utils.checkObstacle(position, stringToIntDir.key(stringToIntDir.value(currentAutoDirection)),
                                      *gameMap))) {
                int temp = depth + 1;

                QPair<int, int> tempPos = utils.returnNextPos(pos, currentDirection);
                qDebug() << tempPos;
                testPaths(rootDirection, stringToIntDir.value(currentAutoDirection), temp, tempPos);
            } else {
                //qDebug() << "PATTTTH";
                if (score.value(rootDirection) < depth) {
                    score.remove(rootDirection);
                    score.insert(rootDirection, depth);
                }
                break;
            }
        }
    }
}

void Worker::getMeThoseNumbers() {
    emit resultReady(id, result.first, result.second);
}

const QPair<int, int> &Worker::getResult() const {
    return result;
}

int Worker::getId() const {
    return id;
}

