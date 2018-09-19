#pragma once

#include <QtCore/QPair>
#include "gameMap.h"

class Utils {

public:
    Utils();
    ~Utils();
    QPair<int, int> returnNextPos(const QPair<int, int> &currentPosition, const int &direction);
    bool checkObstacle(const QPair<int, int> &position, const QString &direction, GameMap &gameMap);
    QPair<int, int> decideBestWay(QMap<int, int> &score);
private:
    QMap<QString, int> stringToIntDir;
};
