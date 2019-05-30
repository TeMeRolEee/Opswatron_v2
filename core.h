#pragma once

#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtCore/QUuid>

#include "clihandler.h"
#include "enginehandler.h"
#include "directions.h"

class Core : public QThread {
Q_OBJECT

public:
	~Core() override;

	bool init();

protected:
	void run() override;

private:
	QTimer *qTimer;

	CliHandler *cliHandler;

	EngineHandler *engineHandler;

	GameMap *gameMap = nullptr;

	int interval = 0;

	int currentBestDirection, maxDistance;

private slots:

	void handleEngineResults_slot(Directions direction, int distance);

	void handleNewTask_slot(const QJsonObject &qJsonObject);

signals:

	void addNewEngine_signal();

	void taskSignal(QPair<int, int> currentPos, Directions direction, int maxDepth, GameMap *gameMap);

	void resetEngines_signal();
};

