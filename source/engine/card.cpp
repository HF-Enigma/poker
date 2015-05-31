#include "card.h"

Card::Card() :
    _suit(SUIT_UNKNOWN),
    _value(0)
{
}

Card::Card(Suit suit, int value) :
    _suit(suit),
    _value(value)
{
}

bool Card::operator>(Card &rhs)
{
    return this->_value > rhs._value;
}

bool Card::operator>=(Card &rhs)
{
    return this->_value >= rhs._value;
}

