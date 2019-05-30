#pragma once

#include <QtCore/QThread>
#include <QtCore/QTimer>

#include "clihandler.h"
#include "enginehandler.h"

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

	EngineHandler *engineHandler = nullptr;

	GameMap *gameMap;

	int interval = 0;
	int width, height;

private slots:

	void handleEngineResults_slot(QUuid uniqueId, const QJsonObject &result);

	void handleNewTask_slot(const QJsonObject &qJsonObject);

	void result_slot(QUuid id);

signals:

	void addNewEngine_signal(const QString &enginePath, const QString &scanParameter, const QString &engineName);

	void startNewScanTask_signal(QUuid uniqueId, QString file);

	void resetEngines_signal();

	void startCalculateResult_signal(QUuid id);

	void startWebServer_signal();
};

