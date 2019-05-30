#pragma once

#include <QtCore/QThread>
#include <QtCore/QMap>
#include "engine.h"
#include "gamemap.h"
#include "directions.h"

class EngineHandler : public QThread {
Q_OBJECT

public:
	EngineHandler();

	~EngineHandler() override;

	int getEngineCount();

protected:
	void run() override;

private:
	QMap<QUuid, Engine *> *engineList;

	QMap<QUuid, QJsonArray *> *resultMap;

	int engineCount = 0;

public slots:

	void handleCalculation_slot(Directions direction, int distance);

	void addNewEngine_slot();

	void handleNewTask_slot(QPair<int, int> currentPos, Directions direction, int maxDepth, GameMap *gameMap);

signals:

	void calculationComplete_signal(Directions direction, int distance);

	QVector<Directions> getAvailableDirections(Directions currentDirection);
};


