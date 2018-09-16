#include "Worker.h"
#include <QDebug>

void Worker::run() {
    QString result;



    qDebug() << QThread::currentThreadId();

    emit resultReady(id, result);
}

QString Worker::findPath() {
    return QString();
}

Worker::Worker(const int &id): id(id) {

}

void Worker::shutDownWorker() {
    emit resultReady(id, currentBestMove);
}

QMap<QPair<int, int>, int> *Worker::setMap(QMap<QPair<int, int>, int> &gameMap) {
    auto tempMap = new QMap<QPair<int,int>,int>;
    *tempMap = gameMap;
    return tempMap;
}
