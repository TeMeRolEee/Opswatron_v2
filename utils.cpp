#include "utils.h"
#include "directions.h"
#include "gameMap.h"

QPair<int, int> Utils::returnNextPos(const QPair<int, int> &currentPosition, const int &direction) {
    switch (direction) {
        case static_cast<int>(Directions::UP):
            return QPair<int, int>(currentPosition.first, currentPosition.second - 1);
        case static_cast<int>(Directions::DOWN):
            return QPair<int, int>(currentPosition.first, currentPosition.second + 1);
        case static_cast<int>(Directions::LEFT):
            return QPair<int, int>(currentPosition.first - 1, currentPosition.second);
        case static_cast<int>(Directions::RIGHT):
            return QPair<int, int>(currentPosition.first + 1, currentPosition.second);
        default:
            break;
    }
}

bool Utils::checkObstacle(const QPair<int, int> &currentPosition, const QString &direction, GameMap &gameMap) {
    int tempValue = gameMap.getPositionInfo(returnNextPos(currentPosition, stringToIntDir.value(direction)));
    if (tempValue >= 0 ) {
        return tempValue == 1;
    }
    return true;
}

Utils::Utils() {
    stringToIntDir = {
            {"UP",    0},
            {"DOWN",  1},
            {"LEFT",  2},
            {"RIGHT", 3}
    };
}

Utils::~Utils() {

}

QPair<int, int> Utils::decideBestWay(QMap<int, int> &score) {
    int bestDirection = stringToIntDir.value("UP");
    int bestScore = score.value(bestDirection);

    for (auto currentDir : score.keys()) {
        if (score.value(currentDir) >= bestScore) {
            bestScore = score.value(currentDir);
            bestDirection = currentDir;
        }
    }
    return QPair<int, int>(bestDirection, bestScore);
}
