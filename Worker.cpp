#include "Worker.h"

#include <QDebug>

#include "Core.h"
#include "directions.h"


Worker::Worker(const int &id, GameMap &gameMap, QMap<int, Player *> &players, const int &direction, const int &depth,
               const QPair<int, int> &position) : id(id),
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
    //qDebug() << "pos" << position;
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
    //qDebug() << "worker_score" << id << ":" << score;
    //qDebug() << "result:" << id << ":" << result;

    done = true;

    //emit resultReady(id, result.first, result.second);
}

void Worker::testPaths(const int &rootDirection, const int &currentDirection, const int &depth, QPair<int, int> &pos) {
    if (depth < 4) {
        //qDebug() << depth;
        /*for (const auto &currentAutoDirection : stringToIntDir.keys()) {
            qDebug() << currentAutoDirection << rootDirection << currentDirection << depth << pos;
        }*/

        for (auto &currentAutoDirection : stringToIntDir) {
            /*qDebug() << "depth:" << depth << "id" << id << QStringLiteral(
                    "currentAutoDirection: %0 != reverseDirection.value(stringToIntDir.key(currentDirection)): %1, stringToIntDir.key(currentDirection) %2").
                    arg(stringToIntDir.key(currentAutoDirection)).arg(reverseDirection.value(stringToIntDir.key(currentDirection))).arg(
                    stringToIntDir.key(currentDirection))
                     << (currentAutoDirection != reverseDirection.value(stringToIntDir.key(currentDirection)))
                     << "yes_or_no"
                     << ((currentAutoDirection != reverseDirection.value(stringToIntDir.key(currentDirection))) &&
                         !(utils.checkObstacle(position, stringToIntDir.key(currentAutoDirection),
                                               *gameMap)))
                     << (utils.checkObstacle(position, stringToIntDir.key(currentAutoDirection),
                                             *gameMap));*/

            //qDebug() << reverseDirection.value(stringToIntDir.key(currentAutoDirection));
            if ((stringToIntDir.key(currentAutoDirection) != reverseDirection.value(stringToIntDir.key(currentAutoDirection))) &&
                !(utils.checkObstacle(position, stringToIntDir.key(currentAutoDirection),
                                      *gameMap))) {
                int temp = depth + 1;

                QPair<int, int> tempPos = utils.returnNextPos(pos, currentDirection);
                //qDebug() << tempPos;
                testPaths(rootDirection, stringToIntDir.value(stringToIntDir.key(currentAutoDirection)), temp, tempPos);
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

bool Worker::isDone() const {
    return done;
}

