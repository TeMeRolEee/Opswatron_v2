#include <iostream>
#include "gameMap.h"

GameMap::GameMap(const int &width, const int &height) :
        width(width),
        height(height) {
    QVector<QVector<int>*> *row = new QVector<QVector<int>*>;
    row->reserve(width);
    for (int y = 0; y < height; y++) {
        QVector<int> *column = new QVector<int>;
        column->reserve(width);
        for (int x = 0; x < width; x++) {
            column->replace(x, 0);
        }
        row->replace(y, column);
    }
    gameMap = std::move(row);
    showMap();
}

GameMap::~GameMap() {
    delete gameMap;
}

void GameMap::showMap() {
    for (int y = 0; y < height; y++) {
        QString line;
        for (int x = 0; x < width; x++) {
            line.append(gameMap->at(y)->at(x));
        }
        std::cout << line.toStdString() << std::endl;
    }
}

void GameMap::placeWall(const int &x, const int &y) {
    gameMap->at(y)->replace(x, 1);
}
