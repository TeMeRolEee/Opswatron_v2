#include "Player.h"

Player::Player() {

}

Player::~Player() {

}

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

