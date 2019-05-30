#include "enginehandler.h"

#include <QtCore/QJsonObject>
#include <QtCore/QUuid>

EngineHandler::EngineHandler() {
	engineList = new QMap<QUuid, Engine *>();
	resultMap = new QMap<QUuid, QJsonArray *>;
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
	connect(this, &EngineHandler::startCalculate_signal, engine, &Engine::startCalculate_slot);
	connect(engine, &Engine::calculationFinised_signal, this, &EngineHandler::handleCalculation_slot);

	engineList->insert(QUuid::createUuidV5(QUuid::createUuid(), (QString)engineCount++), engine);
	engine->start();
}

void EngineHandler::handleNewTask_slot(const GameMap &gameMap) {

}

void EngineHandler::handleCalculation_slot(int direction, int distance) {

}
