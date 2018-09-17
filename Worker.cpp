#include "Worker.h"
#include "Core.h"
#include <QDebug>

Worker::Worker(const int &id, GameMap &gameMap, QMap<int, Player*> &players): id(id) {
    this->gameMap = &gameMap;
    this->players = players;
}

Worker::~Worker() {

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



