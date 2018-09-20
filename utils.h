#pragma once

#include <QtCore/QPair>
#include "gameMap.h"
#include "Player.h"

class Utils {

public:
    Utils();
    ~Utils();
    QPair<int, int> returnNextPos(const QPair<int, int> &currentPosition, const int &direction);
    bool checkObstacle(const QPair<int, int> &position, const QString &direction, GameMap &gameMap);
    bool checkEnemyAhead(const QPair<int, int> &position, const QString &direction, QMap<int, Player *> &players);
    QPair<int, int> decideBestWay(QMap<int, int> &score);
    bool checkMoreSpaceLeft(const int &direction, const QPair<int, int> &currentPosition);
private:
    QMap<QString, int> stringToIntDir;
};
