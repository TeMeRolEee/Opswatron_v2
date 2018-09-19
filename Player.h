#pragma once

#include <QtCore/QString>
#include <QtCore/QPair>

class Player {
public:
    QPair<int, int> getCurrentPosition();

    int getCurrentX() const;

    void setCurrentX(int currentX);

    int getCurrentY() const;

    void setCurrentY(int currentY);

    const QString &getDirection() const;

    void setDirection(const QString &direction);

    bool isAlive() const;

    void setAlive(bool alive);

    const QString &getName() const;

    void setName(const QString &name);

    int getId() const;

    void setId(int id);

private:
    int currentX, currentY;
    QString direction, name;
    bool alive;
    int id;
};

