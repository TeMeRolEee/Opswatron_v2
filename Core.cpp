#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include "Core.h"

void setInterval(int interval) {
    interval = interval;
}

void Core::processData(const QString &input) {
    QJsonObject qJsonObject = QJsonDocument::fromJson(input.toUtf8()).object();

    interval = qJsonObject.value("interval").toInt();

    QJsonArray gameMapArray = qJsonObject.value("map").toArray();

    QJsonArray other_Players = qJsonObject.value("other_players").toArray();
    for (auto player : other_Players) {
        int xPos = player.toObject().value("coords").toArray().first().toInt();
        int yPos = player.toObject().value("coords").toArray().last().toInt();
        //player.toObject().value()
    }

    me.setAlive(qJsonObject.value("player").toObject().value("alive").toBool());
    me.setCurrentX(qJsonObject.value("player").toArray().first().toInt());
    me.setCurrentY(qJsonObject.value("player").toArray().last().toInt());




}
