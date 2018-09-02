#pragma once

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QTimer>
#include "Player.h"

class Core : public QObject {
Q_OBJECT
public:
    void processData(const QString &input);

private:
    QMap<int, Player> players;
    Player me;
    QTimer qTimer;
    int interval;
    int myId;

};
