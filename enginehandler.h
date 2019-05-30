#pragma once

#include <QtCore/QThread>
#include <QtCore/QMap>

class EngineHandler : public QThread {
Q_OBJECT

public:
	EngineHandler();

	~EngineHandler() override;

	int getEngineCount();

protected:
	void run() override;

private:

	bool findExistingEngine(const QString &engineName);

	QMap<int, Engine *> *engineList;
	QMap<QString, int> engineNameList;
	QMap<QString, QJsonArray *> *resultMap;
	QVector<int> *scanIdList;
	int engineCount = 0;
};


