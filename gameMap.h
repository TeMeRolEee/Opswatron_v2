#pragma once

#include <QMap>

class GameMap {
public:
    GameMap(const int &width, const int &height);
    ~GameMap();

    void showMap();
    void placeWall(int x, int y);

    int getPositionInfo(const QPair<int, int> &position);

private:
    int width, height;
    QMap<QPair<int,int>,int> *gameMap;

};

