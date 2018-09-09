#pragma once

#include <QtCore/QVector>

class GameMap {
public:
    GameMap(const int &width, const int &height);
    ~GameMap();

    void showMap();
    void placeWall(int x, int y);

private:
    int width, height;
    QMap<std::pair<int,int>,int> *gameMap;

};

