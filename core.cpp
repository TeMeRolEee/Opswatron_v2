#include "core.h"

#include <QDebug>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QPair>

#include "enginehandler.h"

void Core::run() {
	QThread::run();
}

bool Core::init() {
	qTimer = new QTimer();
	connect(this, &Core::finished, this, &Core::deleteLater);

	cliHandler = new CliHandler();
	connect(cliHandler, &CliHandler::finished, cliHandler, &CliHandler::deleteLater);
	connect(cliHandler, &CliHandler::newTask_signal, this, &Core::handleNewTask_slot, Qt::QueuedConnection);
	connect(this, &Core::stopCliHandler_signal, cliHandler, &CliHandler::stopCli_slot);

	engineHandler = new EngineHandler();
	connect(this, &Core::addNewEngine_signal, engineHandler, &EngineHandler::addNewEngine_slot);
	connect(this, &Core::taskSignal, engineHandler, &EngineHandler::handleNewTask_slot,
			Qt::QueuedConnection);
	connect(engineHandler, &EngineHandler::calculationComplete_signal, this, &Core::handleEngineResults_slot,
			Qt::QueuedConnection);
	connect(engineHandler, &EngineHandler::finished, engineHandler, &EngineHandler::deleteLater);
	engineHandler->start();

	emit addNewEngine_signal();
	emit addNewEngine_signal();
	emit addNewEngine_signal();

	cliHandler->start();

	return true;
}

Core::~Core() {
	qTimer->stop();
	delete qTimer;

	emit cliHandler->stopCli_signal();
	cliHandler->wait();
	cliHandler->quit();
	delete cliHandler;

	engineHandler->wait();
	engineHandler->quit();
	delete engineHandler;

	delete gameMap;
}

void Core::handleNewTask_slot(const QJsonObject &qJsonObject) {
	qDebug() << "WTF";
	if (gameMap == nullptr) {
		QJsonArray array = qJsonObject.value("map").toArray();
		gameMap = new GameMap(array.at(0).toInt(), array.at(1).toInt());
	}

	QJsonObject playerObject = qJsonObject.value("player").toObject();
	gameMap->placeWall(playerObject.value("coords").toArray().at(0).toInt(),
					   playerObject.value("coords").toArray().at(0).toInt());

	QJsonArray otherPlayers = qJsonObject.value("other_players").toArray();

	for (auto player : otherPlayers) {
		QJsonObject otherPlayerObject = player.toObject();
		gameMap->placeWall(otherPlayerObject.value("coords").toArray().at(0).toInt(),
						   otherPlayerObject.value("coords").toArray().at(1).toInt());
	}

	QJsonArray walls = qJsonObject.value("walls").toArray();
	for (auto wall : walls) {
		QJsonObject wallObject = wall.toObject();
		gameMap->placeWall(wallObject.value("coords").toArray().at(0).toInt(),
						   wallObject.value("coords").toArray().at(1).toInt());
	}
	QPair<int, int> position;
	position.first = (playerObject.value("coords").toArray().at(0).toInt());
	position.second = (playerObject.value("coords").toArray().at(1).toInt());
	qDebug() << "new task staring:";
	emit taskSignal(position, getDirection(playerObject.value("dir").toString()), 3, gameMap);
}

void Core::handleEngineResults_slot(Directions direction, int distance) {
	if (maxDistance < distance) {
		currentBestDirection = direction;
	}
}

Directions Core::getDirection(const QString &direction) {
	static QMap<QString, Directions> directions{
			{"UP",    Directions::UP},
			{"DOWN",  Directions::DOWN},
			{"LEFT",  Directions::LEFT},
			{"RIGHT", Directions::RIGHT}
	};

	return directions.value(direction);
}
