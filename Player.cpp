#include "Player.h"

int Player::getCurrentX() const {
    return currentX;
}

void Player::setCurrentX(int currentX) {
    Player::currentX = currentX;
}

int Player::getCurrentY() const {
    return currentY;
}

void Player::setCurrentY(int currentY) {
    Player::currentY = currentY;
}

const QString &Player::getDirection() const {
    return direction;
}

void Player::setDirection(const QString &direction) {
    Player::direction = direction;
}

bool Player::isAlive() const {
    return alive;
}

void Player::setAlive(bool alive) {
    Player::alive = alive;
}

const QString &Player::getName() const {
    return name;
}

void Player::setName(const QString &name) {
    Player::name = name;
}

int Player::getId() const {
    return id;
}

void Player::setId(int id) {
    Player::id = id;
}

QPair<int, int> Player::getCurrentPosition() {
    return QPair<int, int>(currentX, currentY);
}

