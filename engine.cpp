#include "engine.h"
#include <QDebug>

Engine::Engine() {
}

void Engine::run() {
	QThread::run();
}

Engine::~Engine() {
	delete gameMap;
}

void Engine::startCalculate_slot(QPair<int, int> currentPos, Directions direction, int maxDepth, GameMap *gameMap) {
	qDebug() << currentPos.first << currentPos.second << static_cast<int>(direction) << maxDepth;
	gameMap->showMap();
}

