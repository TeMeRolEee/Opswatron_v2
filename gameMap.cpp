#include <iostream>
#include <QDebug>

#include "gameMap.h"
#include "directions.h"

GameMap::GameMap(const int &inputWidth, const int &inputHeight) :
        width(inputWidth),
        height(inputHeight) {
    gameMap = new QMap<QPair<int, int>, int>;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            QPair<int, int> mapPlace(x, y);
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
            QPair<int, int> mapPlace(x, y);
            line += std::to_string(gameMap->value(mapPlace));
        }
        std::cout << line << std::endl;
    }
    std::cout << std::endl << std::endl;
}

void GameMap::placeWall(int x, int y) {
    //qDebug() << x << y;
    gameMap->remove(QPair<int, int>(x, y));
    gameMap->insert(QPair<int, int>(x, y), 1);
    //showMap();
}

int GameMap::getPositionInfo(const QPair<int, int> &position) {
    int x = position.first;
    int y = position.second;
    if ((x >= 0) && (x <= (width - 1)) && ((y >= 0) && (y <= (height - 1)))) {
        return gameMap->value(position);
    }
    return 1;
}

QPair<int, int> GameMap::getSpaceLeft(const QPair<int, int> &position, const int &direction) {
    switch (direction) {
        case static_cast<int>(Directions::UP): {
            int leftSide = 0;
            int rightSide = 0;
            for (int i = position.first; i != 0; i--) {
                if (gameMap->value(QPair<int, int>(i, position.second)) == 0) {
                    leftSide++;
                }
            }
            for (int i = position.first; i != width; i++) {
                if (gameMap->value(QPair<int, int>(i, position.second)) == 0) {
                    rightSide++;
                }
            }

            return QPair<int, int>(leftSide, rightSide);
        }
        case static_cast<int>(Directions::DOWN): {
            int leftSide = 0;
            int rightSide = 0;
            for (int i = position.first; i != 0; i--) {
                if (gameMap->value(QPair<int, int>(i, position.second)) == 0) {
                    leftSide++;
                }
            }
            for (int i = position.first; i != width; i++) {
                if (gameMap->value(QPair<int, int>(i, position.second)) == 0) {
                    rightSide++;
                }
            }
            return QPair<int, int>();
        }
        case static_cast<int>(Directions::LEFT): {
            int upSide = 0;
            int downSide = 0;
            for (int i = position.first; i != 0; i--) {
                if (gameMap->value(QPair<int, int>(position.first, i)) == 0) {
                    upSide++;
                }
            }
            for (int i = position.first; i != width; i++) {
                if (gameMap->value(QPair<int, int>(position.first, i)) == 0) {
                    downSide++;
                }
            }
            return QPair<int, int>(upSide, downSide);
        }
        case static_cast<int>(Directions::RIGHT): {
            int upSide = 0;
            int downSide = 0;
            for (int i = position.first; i != 0; i--) {
                if (gameMap->value(QPair<int, int>(position.first, i)) == 0) {
                    upSide++;
                }
            }
            for (int i = position.first; i != width; i++) {
                if (gameMap->value(QPair<int, int>(position.first, i)) == 0) {
                    downSide++;
                }
            }
            return QPair<int, int>(upSide, downSide);
        }
        default:
            break;
    }
    return QPair<int, int>(0, 0);
}
