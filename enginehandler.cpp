#include "enginehandler.h"

#include <QtCore/QJsonObject>
#include <QtCore/QUuid>
#include <QtCore/QVector>

EngineHandler::EngineHandler() {
	engineList = new QMap<QUuid, Engine *>();
	resultMap = new QMap<QUuid, QJsonArray *>;
	//connect(this, &EngineHandler::handleCalculation_slot, this, &EngineHandler::calculationComplete_signal);
}

EngineHandler::~EngineHandler() {
	delete engineList;
	delete resultMap;
}

void EngineHandler::run() {
	QThread::run();
}

void EngineHandler::addNewEngine_slot() {
	auto engine = new Engine();

	connect(engine, &Engine::finished, engine, &Engine::deleteLater);
	connect(engine, &Engine::startCalculate_signal, engine, &Engine::startCalculate_slot);
	connect(engine, &Engine::calculationFinished_signal, this, &EngineHandler::handleCalculation_slot);

	engineList->insert(QUuid::createUuidV5(QUuid::createUuid(), (QString) engineCount++), engine);
	engine->start();
}

void EngineHandler::handleNewTask_slot(QPair<int, int> currentPos, Directions direction, int maxDepth, GameMap *gameMap) {
	static QMap<Directions, Directions> reverseDirections = {
			{Directions::UP,    Directions::DOWN},
			{Directions::DOWN,  Directions::UP},
			{Directions::LEFT,  Directions::RIGHT},
			{Directions::RIGHT, Directions::LEFT}
	};

	auto *goodDirections = new QVector<Directions>();

	for (auto currentDirection : reverseDirections) {
		if (reverseDirections.value(currentDirection) != currentDirection) {
			goodDirections->append(currentDirection);
		}
	}

	for (auto engineKey : engineList->keys()) {
		emit engineList->value(engineKey)->startCalculate_slot(currentPos,
															   goodDirections->takeAt(0),
															   maxDepth,
															   gameMap);
	}

	delete goodDirections;
}

void EngineHandler::handleCalculation_slot(Directions direction, int distance) {
	emit calculationComplete_signal(direction, distance);
}

int EngineHandler::getEngineCount() {
	return engineCount;
}

