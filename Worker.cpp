#include "Worker.h"
#include "Core.h"
#include <QDebug>

Worker::Worker(const int &id, GameMap &gameMap, QMap<int, Player*> &players, const int &direction): id(id),
    gameMap(&gameMap),
    players(players),
    direction(direction) {
}

Worker::~Worker() {
    delete gameMap;
}

void Worker::run() {
    QString result;


    qDebug() << QThread::currentThreadId();

    emit resultReady(id, result);
}

QString Worker::findPath() {
    return QString();
}

void Worker::shutDownWorker() {
    emit resultReady(id, currentBestMove);
}



