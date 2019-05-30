#pragma once

#include <QtCore/QThread>
#include "gamemap.h"

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

	void startCalculate_slot(QPair<int, int> currentPos, int direction, int maxDepth, GameMap *gameMap);

signals:

	void calculationFinised_signal(int direction, int distance);
};


