#pragma once

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QTimer>

#include "Player.h"
#include "gameMap.h"
#include "Worker.h"
#include "utils.h"

class Core : public QObject {
Q_OBJECT

public:
    Core();

    ~Core();

    void initCore(const QString &input);

    void processData(const QJsonObject &qJsonObject);

    Player *createPlayer(const QJsonObject &playerData);

public:
    GameMap *getGameMap() const;

    const QMap<int, Player *> &getPlayers() const;

    const Player &getMe() const;

    void think();

    int getInterval() const;

private:
    Utils utils;

    GameMap *gameMap;

    QMap<int, Player *> players;

    Player me;

    QTimer *qTimer;

    int interval = 0;

private:

    int workerCount = 0;

    int workerFinished = 0;

    QMap<int, Worker *> workers;

    QMap<QString, int> stringToIntDir;

    QMap<QString, QString> reverseDirection;

    QMap<int, int> score;

    void createWorkers(const QVector<int> &directions);

    void printNextDirection(const int &direction);

public slots:

    void handleResults(const int &id, const int &direction, const int &score);

signals:

    void getResultNow(const int &id);
};
