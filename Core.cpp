#include <chrono>

#include <QDebug>

#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>

#include "Core.h"

Core::Core() {
    qTimer = new QTimer();
}

Core::~Core() {
    qTimer->stop();
    delete qTimer;
    delete gameMap;
}

void Core::initCore(const QString &input) {
    QJsonObject qJsonObject = QJsonDocument::fromJson(input.toUtf8()).object();
    interval = qJsonObject.value("interval").toInt();

    QJsonArray gameMapArray = qJsonObject.value("map").toArray();
    gameMap = new GameMap(gameMapArray.first().toInt(), gameMapArray.last().toInt());

    QJsonArray other_Players = qJsonObject.value("other_players").toArray();
    for (auto player : other_Players) {
        players.insert(player.toObject().value("id").toInt(), createPlayer(player.toObject()));
    }
}

void Core::processData(const QJsonObject &qJsonObject) {
    auto point1 = std::chrono::high_resolution_clock::now();

    if (interval != qJsonObject.value("interval").toInt()) {
        interval = qJsonObject.value("interval").toInt();
        qTimer->setInterval(interval);
    }

    QJsonArray other_Players = qJsonObject.value("other_players").toArray();
    for (auto player : other_Players) {
        if (player.toObject().value("alive").toBool()) {
            int playerId = player.toObject().value("id").toInt();
            QJsonArray coordsArray = player.toObject().value("coords").toArray();
            players.value(playerId)->setCurrentX(coordsArray.first().toInt());
            players.value(playerId)->setCurrentY(coordsArray.last().toInt());
            players.value(playerId)->setDirection(player.toObject().value("dir").toString());
        }
    }

    me.setAlive(qJsonObject.value("player").toObject().value("alive").toBool());
    QJsonArray playerPosArray = qJsonObject.value("player").toArray();
    me.setCurrentX(playerPosArray.first().toInt());
    me.setCurrentY(playerPosArray.last().toInt());

    QJsonArray wallArray = qJsonObject.value("walls").toArray();
    for (auto bricks : wallArray) {
        QJsonArray brickArray = bricks.toObject().value("coords").toArray();
        gameMap->placeWall(brickArray.first().toInt(),
                           brickArray.last().toInt());
    }

    auto point2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(point2 - point1).count();
    qDebug() << "time needed to process data:" << duration << "ms";
}

Player *Core::createPlayer(const QJsonObject &playerData) {
    auto *actualPlayer = new Player();

    actualPlayer->setCurrentX(playerData.value("coords").toArray().first().toInt());
    actualPlayer->setCurrentY(playerData.value("coords").toArray().last().toInt());
    actualPlayer->setAlive(playerData.value("alive").toBool());
    actualPlayer->setDirection(playerData.value("dir").toString());
    actualPlayer->setId(playerData.value("id").toInt());
    actualPlayer->setName(playerData.value("name").toString());

    return actualPlayer;
}


