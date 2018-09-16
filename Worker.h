#pragma once

#include <QtCore/QThread>

class Worker: public QThread {
Q_OBJECT

public:
    Worker(const int &id);

    QMap<QPair<int,int>,int> *setMap(QMap<QPair<int,int>,int> &gameMap);

protected:
    void run() override;

private:
    int id;

    bool isRunning;

    QString findPath();

    QString currentBestMove;

public slots:
    void shutDownWorker();

signals:
    void resultReady(const int &id ,const QString &qString);
};
