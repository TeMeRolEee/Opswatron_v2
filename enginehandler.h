#pragma once

#include <QtCore/QThread>
#include <QtCore/QMap>
#include "engine.h"
#include "gamemap.h"

class EngineHandler : public QThread {
Q_OBJECT

public:
	EngineHandler();

	~EngineHandler() override;

protected:
	void run() override;

private:
	QMap<QUuid, Engine *> *engineList;

	QMap<QUuid, QJsonArray *> *resultMap;

	int engineCount = 0;

public slots:

	void handleCalculation_slot(int direction, int distance);

	void addNewEngine_slot();

	void handleNewTask_slot(const GameMap &gameMap);

signals:

	void startCalculate_signal(QPair<int, int>, int);

};


