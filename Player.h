#pragma once

#include <QtCore/QString>

class Player {
public:
    Player();

    ~Player();

    int getCurrentX() const;

    void setCurrentX(int currentX);

    int getCurrentY() const;

    void setCurrentY(int currentY);

    const QString &getDirection() const;

    void setDirection(const QString &direction);
    bool isAlive() const;

    void setAlive(bool alive);

private:
    int currentX, currentY;
    QString direction;
    bool alive;
};

