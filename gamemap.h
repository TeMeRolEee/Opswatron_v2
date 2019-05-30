#pragma once

#include <QtCore/QThread>
#include <QtCore/QMap>

class GameMap : public QThread {
Q_OBJECT

public:
	GameMap(const int &width, const int &height);

	~GameMap();

	void showMap();

	void placeWall(int x, int y);

	int getPositionInfo(const QPair<int, int> &position);

	QPair<int, int> getSpaceLeft(const QPair<int, int> &position, const int &direction);

private:
	int width, height;
	QMap<QPair<int, int>, int> gameMap;
};


