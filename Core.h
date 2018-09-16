#pragma once

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QTimer>
#include "Player.h"
#include "gameMap.h"
#include "Worker.h"

class Core : public QObject {
Q_OBJECT

public:
    Core();
    ~Core();
    void initCore(const QString &input);
    void processData(const QJsonObject &qJsonObject);
    Player *createPlayer(const QJsonObject &playerData);

private:
    GameMap *gameMap;
    QMap<int, Player*> players;
    Player me;
    QTimer *qTimer;
    int interval = 0;
    int workerCount = 0;
    QMap<int, Worker*> workers;
    QMap<int, QStringList*> workerResults;

public slots:
    void handleResults(const int &id, const QString &qString);

signals:
    void getResultNow(const int &id);
};
