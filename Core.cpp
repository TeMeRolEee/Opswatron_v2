#include <chrono>

#include <QDebug>

#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <iostream>

#include "Core.h"
#include "directions.h"

Core::Core() {
    qTimer = new QTimer();
    reverseDirection = {
            {"UP",    "DOWN"},
            {"DOWN",  "UP"},
            {"LEFT",  "RIGHT"},
            {"RIGHT", "LEFT"}
    };
    stringToIntDir = {
            {"UP",    0},
            {"DOWN",  1},
            {"LEFT",  2},
            {"RIGHT", 3}
    };
    score = {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 0}
    };
}

Core::~Core() {
    qTimer->stop();
    delete qTimer;
    delete gameMap;
}

void Core::initCore(const QString &input) {
    QJsonObject qJsonObject = QJsonDocument::fromJson(input.toUtf8()).object();
    interval = qJsonObject.value("interval").toInt()-5;
    qTimer->start();
    QJsonArray gameMapArray = qJsonObject.value("map").toArray();
    gameMap = new GameMap(gameMapArray.first().toInt(), gameMapArray.last().toInt());

    QJsonArray other_Players = qJsonObject.value("other_players").toArray();
    for (auto player : other_Players) {
        players.insert(player.toObject().value("id").toInt(), createPlayer(player.toObject()));
    }

    QVector<int> possibleDirections;
    int currentReverseDirection = stringToIntDir.value(reverseDirection.value(me.getDirection()));

    for (const auto &value : stringToIntDir) {
        if ((value != currentReverseDirection) &&
            (utils.checkObstacle(QPair<int, int>(me.getCurrentX(), me.getCurrentY()), stringToIntDir.key(value),
                                 *gameMap))) {
            possibleDirections.push_back(value);
        }
    }

}

void Core::processData(const QJsonObject &qJsonObject) {
    //auto point1 = std::chrono::high_resolution_clock::now();

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

    //auto point2 = std::chrono::high_resolution_clock::now();
    //auto duration = std::chrono::duration_cast<std::chrono::microseconds>(point2 - point1).count();
    //qDebug() << "time needed to process data:" << duration << "ms";

    QVector<int> possibleDirections;
    int currentReverseDirection = stringToIntDir.value(reverseDirection.value(me.getDirection()));

    for (const auto &value : stringToIntDir) {
        if ((value != currentReverseDirection) &&
            (utils.checkObstacle(me.getCurrentPosition(), stringToIntDir.key(value), *gameMap))) {
            possibleDirections.push_back(value);
        }
    }

    createWorkers(possibleDirections);

    int bestDirection = stringToIntDir.value(me.getDirection());
    int bestScore = score.value(bestDirection);
    for (auto currentDir : score.keys()) {
        if(score.value(currentDir) >  bestScore ) {
            bestScore = score.value(currentDir);
            bestDirection = currentDir;
        }
    }

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

void Core::handleResults(const int &id, const int &inputScore) {
    workers.value(id)->quit();
    workers.value(id)->wait();

    score[id] = inputScore;

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

void Core::createWorkers(const QVector<int> &directions) {
    int currentDirection = stringToIntDir.value(me.getDirection());

    for (auto currentPossibleDirection : directions) {
        auto *worker = new Worker(workerCount, *gameMap, players, currentPossibleDirection, 0,
                                  static_cast<int>(interval * 0.95),
                                  QPair<int, int>(me.getCurrentX(), me.getCurrentY()));

        workers.insert(workerCount, worker);
        workerCount++;

        connect(worker, &Worker::resultReady, this, &Core::handleResults);
        connect(this, &Core::getResultNow, worker, &Worker::shutDownWorker);
        connect(qTimer, &QTimer::timeout, worker, &Worker::shutDownWorker);
        worker->start();
    }
}

void Core::printNextDirection(const int &direction) {
    switch (direction) {
        case static_cast<int>(Directions::UP):
            std::cout << QStringLiteral("{\"dir\":\"UP\"}").arg(stringToIntDir.key(direction)).toStdString() << std::endl;
        case static_cast<int>(Directions::DOWN):
            std::cout << QStringLiteral("{\"dir\":\"DOWN\"}").arg(stringToIntDir.key(direction)).toStdString() << std::endl;
        case static_cast<int>(Directions::LEFT):
            std::cout << QStringLiteral("{\"dir\":\"LEFT\"}").arg(stringToIntDir.key(direction)).toStdString() << std::endl;
        case static_cast<int>(Directions::RIGHT):
            std::cout << QStringLiteral("{\"dir\":\"RIGHT\"}").arg(stringToIntDir.key(direction)).toStdString() << std::endl;
        default:
            break;
    }
}


