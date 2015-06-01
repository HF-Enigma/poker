#include "player.h"

Player::Player() :
    name("unknown"),
    folded(false),
    allin(false)
{
}

Player::Player(std::string &name) :
    name(name),
    folded(false),
    allin(false)
{
}

int Player::getMoney() const
{
    return money;
}

void Player::setMoney(int value)
{
    money = value;
}

int Player::getChips() const
{
    return chips;
}

void Player::setChips(int value)
{
    chips = value;
}

Action Player::getLastAction() const
{
    return lastAction;
}

void Player::setLastAction(const Action &value)
{
    lastAction = value;
}

int Player::getId() const
{
    return id;
}

void Player::setId(int value)
{
    id = value;
}







