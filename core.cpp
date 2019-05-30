#include "core.h"
#include "enginehandler.h"


void Core::run() {
	QThread::run();
}

bool Core::init() {
	connect(this, &Core::finished, this, &Core::deleteLater);

	cliHandler = new CliHandler();
	connect(cliHandler, &CliHandler::newTask_signal, this, &Core::handleNewTask_slot);

	engineHandler = new EngineHandler();
	connect(this, &Core::addNewEngine_signal, engineHandler, &EngineHandler::addNewEngine_slot);
	connect(this, &Core::startNewScanTask_signal, engineHandler, &EngineHandler::handleNewTask_slot,
			Qt::QueuedConnection);
	connect(engineHandler, &EngineHandler::scanComplete_signal, this, &Core::handleEngineResults_slot,
			Qt::QueuedConnection);
	connect(engineHandler, &EngineHandler::finished, engineHandler, &EngineHandler::deleteLater);
	engineHandler->start();
	return false;
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
}
