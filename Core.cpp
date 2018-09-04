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

void Core::processData(const QString &input, const bool &initMode) {
    QJsonObject qJsonObject = QJsonDocument::fromJson(input.toUtf8()).object();

    if (interval != qJsonObject.value("interval").toInt()) {
        interval = qJsonObject.value("interval").toInt();
        qTimer->setInterval(interval);
    }

    QJsonArray gameMapArray = qJsonObject.value("map").toArray();
    if (initMode) {
        gameMap = new GameMap(gameMapArray.first().toInt(), gameMapArray.last().toInt());
    }

    QJsonArray other_Players = qJsonObject.value("other_players").toArray();
    for (auto player : other_Players) {
        if(initMode) {
            players.insert(player.toObject().value("id").toInt(), createPlayer(player.toObject()));
        } else {
            if (player.toObject().value("alive").toBool()) {
                players.value(player.toObject().value("id").toInt())->setCurrentX(player.toObject().value("coords").toArray().first().toInt());
                players.value(player.toObject().value("id").toInt())->setCurrentY(player.toObject().value("coords").toArray().last().toInt());
                players.value(player.toObject().value("id").toInt())->setDirection(player.toObject().value("dir").toString());
            }
        }
    }

    me.setAlive(qJsonObject.value("player").toObject().value("alive").toBool());
    me.setCurrentX(qJsonObject.value("player").toArray().first().toInt());
    me.setCurrentY(qJsonObject.value("player").toArray().last().toInt());

}

Player *Core::createPlayer(const QJsonObject &playerData) {
    Player *actualPlayer = new Player();

    actualPlayer->setCurrentX(playerData.value("coords").toArray().first().toInt());
    actualPlayer->setCurrentY(playerData.value("coords").toArray().last().toInt());
    actualPlayer->setAlive(playerData.value("alive").toBool());
    actualPlayer->setDirection(playerData.value("dir").toString());
    actualPlayer->setId(playerData.value("id").toInt());
    actualPlayer->setName(playerData.value("name").toString());

    return actualPlayer;
}


