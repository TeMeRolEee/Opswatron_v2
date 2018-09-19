#include "utils.h"
#include "directions.h"
#include "gameMap.h"

QPair<int, int> Utils::returnNextPos(const QPair<int, int> &currentPosition, const int &direction) {
    switch (direction) {
        case static_cast<int>(Directions::UP):
            return QPair<int, int>(currentPosition.first - 1, currentPosition.second);
        case static_cast<int>(Directions::DOWN):
            return QPair<int, int>(currentPosition.first + 1, currentPosition.second);
        case static_cast<int>(Directions::LEFT):
            return QPair<int, int>(currentPosition.first, currentPosition.second - 1);
        case static_cast<int>(Directions::RIGHT):
            return QPair<int, int>(currentPosition.first - 1, currentPosition.second + 1);
        default:
            break;
    }
}

bool Utils::checkObstacle(const QPair<int, int> &currentPosition, const QString &direction, GameMap &gameMap) {
    return gameMap.getPositionInfo(returnNextPos(currentPosition, stringToIntDir.value(direction))) == 1;
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
