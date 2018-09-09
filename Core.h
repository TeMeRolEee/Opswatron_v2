#pragma once

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QTimer>
#include "Player.h"

class Core : public QObject {
Q_OBJECT
public:
    Core();
    ~Core();
    void processData(const QString &input, const bool &initMode);
    Player *createPlayer(const QJsonObject &playerData);
private:
    QMap<int, Player*> players;
    Player me;
    QTimer *qTimer;
    int interval;
    int myId;

};
