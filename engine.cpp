#include "engine.h"

Engine::Engine() {
}

void Engine::run() {
	QThread::run();
}

Engine::~Engine() {
	delete gameMap;
}

void Engine::startCalculate_slot(QPair<int, int> currentPos, int direction, int maxDepth, GameMap *gameMap) {

}

