#pragma once

#include <QtCore/QThread>
#include "gamemap.h"
#include "directions.h"

class Engine : public QThread {
Q_OBJECT

public:
	explicit Engine();

	~Engine() override;

protected:
	void run() override;

private:
	GameMap *gameMap;

public slots:

	void startCalculate_slot(QPair<int, int> currentPos, Directions direction, int maxDepth, GameMap *gameMap);

signals:

	void startCalculate_signal(QPair<int, int> currentPos, Directions direction, int maxDepth, GameMap *gameMap);

	void calculationFinished_signal(Directions direction, int distance);
};


