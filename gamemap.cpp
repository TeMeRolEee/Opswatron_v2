#include "gamemap.h"

#include <iostream>
#include <QDebug>

GameMap::GameMap(const int &inputWidth, const int &inputHeight) :
		width(inputWidth),
		height(inputHeight) {
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			QPair<int, int> mapPlace(x, y);
			gameMap.insert(mapPlace, 0);
		}
	}
	showMap();
}

GameMap::~GameMap() {
}

void GameMap::showMap() {
	for (int y = 0; y < height; y++) {
		std::string line;
		for (int x = 0; x < width; x++) {
			QPair<int, int> mapPlace(x, y);
			line += std::to_string(gameMap.value(mapPlace));
		}
		qDebug() << QString::fromStdString(line);
	}
	qDebug() << "\n\n\n";
}

void GameMap::placeWall(int x, int y) {
	qDebug() << x << y;
	gameMap.remove(QPair<int, int>(x, y));
	gameMap.insert(QPair<int, int>(x, y), 1);
	showMap();
}

int GameMap::getPositionInfo(const QPair<int, int> &position) {
	int x = position.first;
	int y = position.second;
	if ((x >= 0) && (x <= (width - 1)) && ((y >= 0) && (y <= (height - 1)))) {
		return gameMap.value(position);
	}
	return 1;
}

QPair<int, int> GameMap::getSpaceLeft(const QPair<int, int> &position, const int &direction) {
	return QPair<int, int>();
}
