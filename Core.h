#pragma once

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QTimer>
#include "Player.h"
#include "gameMap.h"

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
};
