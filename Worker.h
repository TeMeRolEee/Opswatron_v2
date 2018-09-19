#pragma once

#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtCore/QVector>

#include "Player.h"
#include "gameMap.h"
#include "utils.h"

class Worker : public QThread {
Q_OBJECT

public:
    Worker(const int &id, GameMap &gameMap, QMap<int, Player *> &players, const int &direction, const int &depth, const int &interval, const QPair<int, int> &position);

    ~Worker();

protected:
    void run() override;

private:
    Utils utils;

    QPair<int, int> position;

    GameMap *gameMap;

    QMap<int, Player *> players;

    QMap<int, int> score;

    QPair<int, int> result;
public:
    const QPair<int, int> &getResult() const;

private:

    void testPaths(const int &rootDirection, const int &currentDirection, const int &depth, QPair<int, int> &pos);

    QString direction;

    QMap<QString, int> stringToIntDir;

    QMap<QString, QString> reverseDirection;

    int workerCount, id, currentDepth;
public:
    int getId() const;


public slots:
    void getMeThoseNumbers();

signals:
    void resultReady(const int &id, const int &direction, const int &score);
};
