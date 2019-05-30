#pragma once

#include <QtCore/QThread>
#include <QtCore/QTimer>

#include "clihandler.h"

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

	int interval = 0;

private slots:

	void handleEngineResults_slot(QUuid uniqueId, const QJsonObject &result);

	void handleNewTask_slot(const QString &input);

	void result_slot(QUuid id);

signals:

	void addNewEngine_signal(const QString &enginePath, const QString &scanParameter, const QString &engineName);

	void startNewScanTask_signal(QUuid uniqueId, QString file);

	void resetEngines_signal();

	void startCalculateResult_signal(QUuid id);

	void startWebServer_signal();
};

