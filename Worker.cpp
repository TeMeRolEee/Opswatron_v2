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
}

Worker::~Worker() {
}

void Worker::run() {
    QVector<int> possibleDirections;
    int currentReverseDirection = stringToIntDir.value(reverseDirection.value(direction));

    for (const auto &value : stringToIntDir) {
        QString rootDirection = stringToIntDir.key(value);
        QString backwardsDirection = stringToIntDir.key(currentReverseDirection);
        bool obstacleCheck = utils.checkObstacle(position, rootDirection, *gameMap);

        if ((rootDirection != backwardsDirection) && !obstacleCheck) {
            testPaths(value, value, currentDepth, position);
        }
    }
    result = utils.decideBestWay(score);
    done = true;

}

void Worker::testPaths(const int &rootDirection, const int &currentDirection, const int &depth, QPair<int, int> &pos) {
    int maxDepth = 5;
    if (depth < maxDepth) {
        for (auto &currentAutoDirection : stringToIntDir) {
            QString rootDirectionString = stringToIntDir.key(currentAutoDirection);
            QString backwardsDirection = reverseDirection.value(rootDirectionString);

            if ((rootDirectionString != backwardsDirection) &&
                !(utils.checkObstacle(position, rootDirectionString, *gameMap)) &&
                utils.checkEnemyAhead(pos, stringToIntDir.key(currentAutoDirection), players)) {
                int temp = depth + 1;
                if (temp >= maxDepth) {
                    if (score.value(rootDirection) < depth) {
                        score.remove(rootDirection);
                        score.insert(rootDirection, depth);
                    }
                    break;
                }

                QPair<int, int> tempPos = utils.returnNextPos(pos, currentDirection);
                testPaths(rootDirection, currentAutoDirection, temp, tempPos);
            } else {
                if (score.value(rootDirection) < depth) {
                    score.remove(rootDirection);
                    score.insert(rootDirection, depth);
                }
                break;
            }
        }
    }
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

