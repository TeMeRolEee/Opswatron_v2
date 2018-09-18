#include <chrono>

#include <QDebug>

#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>

#include "Core.h"
#include "directions.h"

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

    createWorkers();

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

void Core::handleResults(const int &id, const QString &qString) {
    workers.value(id)->quit();
    workers.value(id)->wait();

    delete workers.take(id);
}

GameMap *Core::getGameMap() const {
    return gameMap;
}

const QMap<int, Player *> &Core::getPlayers() const {
    return players;
}

const Player &Core::getMe() const {
    return me;
}

void Core::createWorkers() {
    workerCount = 0;

    workers.clear();

    int currentDirection;
    QString direction = me.getDirection();

    if (direction == "UP") {
        currentDirection = 0;
    } else if (direction == "DOWN") {
        currentDirection = 1;
    } else if (direction == "LEFT") {
        currentDirection = 2;
    } else {
        currentDirection = 3;
    }

    QMap<QString, int> stringToIntDir= {
            {"UP", 0},
            {"DOWN", 1},
            {"LEFT", 2},
            {"RIGHT", 3}
    };

    QVector<int> directionsVector;
    directionsVector.push_back(0);
    directionsVector.push_back(1);
    directionsVector.push_back(2);
    directionsVector.push_back(3);
    qDebug() << currentDirection;

    for (int i = 0; i < 3; i++) {
        if (currentDirection != directionsVector[i]) {
            auto *worker = new Worker(workerCount, *gameMap, players, directionsVector[i], 0);

            workers.insert(workerCount, worker);
            workerCount++;

            connect(worker, &Worker::resultReady, this, &Core::handleResults);
            connect(this, &Core::getResultNow, worker, &Worker::shutDownWorker);
            connect(qTimer, &QTimer::timeout, worker, &Worker::shutDownWorker);
            worker->start();
        }
    }
}


