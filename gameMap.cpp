#include <iostream>
#include <QDebug>

#include "gameMap.h"

GameMap::GameMap(const int &inputWidth, const int &inputHeight):
    width(inputWidth),
    height(inputHeight) {
    gameMap = new QMap<std::pair<int,int>, int>;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            std::pair<int,int> mapPlace (x,y);
            gameMap->insert(mapPlace, 0);
        }
    }
}

GameMap::~GameMap() {
    delete gameMap;
}

void GameMap::showMap() {
    for (int y = 0; y < height; y++) {
        std::string line;
        for (int x = 0; x < width; x++) {
            std::pair<int,int> mapPlace (x,y);
            line += std::to_string(gameMap->value(mapPlace));
        }
        std::cout << line << std::endl;
    }
    std::cout << std::endl << std::endl;
}

void GameMap::placeWall(int x,int y) {
    //qDebug() << x << y;
    gameMap->remove(std::pair<int,int>(x,y));
    gameMap->insert(std::pair<int,int>(x,y),1);
    //showMap();
}
