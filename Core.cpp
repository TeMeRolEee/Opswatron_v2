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
    interval = qJsonObject.value("interval").toInt() - 5;
    qTimer->start();
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
        interval = static_cast<int>(qJsonObject.value("interval").toInt() * 0.8);
        qTimer->setInterval(interval);
    }
    qTimer->start();

    QJsonArray other_Players = qJsonObject.value("other_players").toArray();

    for (auto player : other_Players) {
        if (player.toObject().value("alive").toBool()) {
            int playerId = player.toObject().value("id").toInt();
            QJsonArray coordsArray = player.toObject().value("coords").toArray();
            players.value(playerId)->setCurrentX(coordsArray.first().toInt());
            players.value(playerId)->setCurrentY(coordsArray.last().toInt());
            players.value(playerId)->setDirection(player.toObject().value("dir").toString());
            qDebug() << playerId << "<-id";
        }
    }

    me.setAlive(qJsonObject.value("player").toObject().value("alive").toBool());
    QJsonArray playerPosArray = qJsonObject.value("player").toArray();
    me.setCurrentX(playerPosArray.at(0).toInt());
    me.setCurrentY(playerPosArray.at(1).toInt());

    qDebug() << me.getCurrentPosition() << "|" << other_Players.size() << "|";
    qDebug() << me.getCurrentX() << me.getCurrentY();

    QJsonArray wallArray = qJsonObject.value("walls").toArray();
    for (auto bricks : wallArray) {
        QJsonArray brickArray = bricks.toObject().value("coords").toArray();
        gameMap->placeWall(brickArray.first().toInt(),
                           brickArray.last().toInt());
    }

    auto point2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(point2 - point1).count();

    QVector<int> possibleDirections;
    int currentReverseDirection = stringToIntDir.value(reverseDirection.value(me.getDirection()));

    for (const auto &value : stringToIntDir) {
        //qDebug() << me.getDirection() << value << ((stringToIntDir.key(value) != stringToIntDir.key(currentReverseDirection)) &&
        //                                           !(utils.checkObstacle(me.getCurrentPosition(), stringToIntDir.key(value), *gameMap)));
        if ((stringToIntDir.key(value) != stringToIntDir.key(currentReverseDirection)) &&
            !(utils.checkObstacle(me.getCurrentPosition(), stringToIntDir.key(value), *gameMap))) {
            //qDebug() << value;
            possibleDirections.push_back(value);
        }
    }

    createWorkers(possibleDirections);

    think();
    //qDebug() << "time needed to process data:" << duration << "ms";

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

void Core::handleResults(const int &id, const int &direction, const int &inputScore) {
    //qDebug() << "deleting thread:" << id << direction << inputScore;
    workerFinished++;
    if (score.value(direction) < inputScore) {
        score[direction] = inputScore;
    }
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
    int currentReverseDirection = stringToIntDir.value(reverseDirection.value(me.getDirection()));
    int i = 0;
    for (auto currentPossibleDirection : directions) {
            //qDebug() << currentPossibleDirection << "false or not:"
            //     << (stringToIntDir.key(currentPossibleDirection) != stringToIntDir.key(currentReverseDirection))
            //     << stringToIntDir.key(currentPossibleDirection) << stringToIntDir.key(currentReverseDirection);
            auto *worker = new Worker(workerCount, *gameMap, players, currentPossibleDirection, 0,
                                      QPair<int, int>(me.getCurrentX(), me.getCurrentY()));
            workers.insert(workerCount, worker);
            workerCount++;
            connect(this, &Core::getResultNow, worker, &Worker::getMeThoseNumbers);
            connect(worker, &Worker::resultReady, this, &Core::handleResults);
            worker->start(QThread::HighestPriority);
    }
}

void Core::printNextDirection(const int &direction) {
    //qDebug() << direction;
    switch (direction) {
        case static_cast<int>(Directions::UP):
            std::cout << QStringLiteral("{\"dir\": \"UP\"}").toStdString()
                      << std::endl;
            break;
        case static_cast<int>(Directions::DOWN):
            std::cout << QStringLiteral("{\"dir\": \"DOWN\"}").toStdString()
                      << std::endl;
            break;
        case static_cast<int>(Directions::LEFT):
            std::cout << QStringLiteral("{\"dir\": \"LEFT\"}").toStdString()
                      << std::endl;
            break;
        case static_cast<int>(Directions::RIGHT):
            std::cout << QStringLiteral("{\"dir\": \"RIGHT\"}").toStdString()
                      << std::endl;
            break;
        default:
            break;
    }
}

int Core::getInterval() const {
    return interval;
}

void Core::think() {
    auto point1 = std::chrono::high_resolution_clock::now();
    for (auto worker : workers) {
        //qDebug() << worker->getId() << worker->getResult();

        while(!worker->isDone()) {
            QThread::msleep(3);
        }

        int id = worker->getResult().first;
        int scoreNumber = worker->getId();
        if (score.value(id) < scoreNumber) {
            score.remove(id);
            score.insert(id, scoreNumber);
        }
    }

    //qDebug() << score;
    QPair<int, int> result;
    //qDebug() << score;
    result = utils.decideBestWay(score);
    //qDebug() << result.first;
    printNextDirection(result.first);

    auto point2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(point2 - point1).count();

    for (auto id : workers.keys()) {
        workers.value(id)->quit();
        workers.value(id)->wait();

        delete workers.take(id);
    }
    score.clear();
    score = {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 0}
    };
    //qDebug() << "sdaasdaswewqeqwe234234234";
}


