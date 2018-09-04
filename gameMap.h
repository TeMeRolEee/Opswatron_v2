#pragma once

#include <QtCore/QVector>

class GameMap {
public:
    GameMap(const int &width, const int &height);
    ~GameMap();

    void showMap();
    void placeWall(const int &x, const int &y);
private:
    int width, height;
    QVector<QVector<int>*> *gameMap;

};

