#ifndef CARD_H
#define CARD_H

enum Suit
{
    SUIT_UNKNOWN,
    SUIT_CLUBS,
    SUIT_DIAMONDS,
    SUIT_HEARTS,
    SUIT_SPADES,
};


class Card
{
public:
    Card();
    Card(Suit suit, int value);

    bool operator>(Card &rhs);
    bool operator>=(Card &rhs);

private:
    Suit _suit;
    int _value;
};

#endif // CARD_H
