#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

#include "card.h"

enum ActionFlag
{
    ACTION_BLIND,
    ACTION_FOLD,
    ACTION_CHECK,
    ACTION_CALL,
    ACTION_RAISE // Requires amount to be given, and amount must be amount of chips moved to table, not the amount raises with.
};

struct Action
{
    ActionFlag command;
    int amount;

    Action(ActionFlag command, int amount = 0) :
        command(command),
        amount(amount)
    {

    }
    Action() :
        command(ACTION_FOLD),
        amount(0)
    {}
};

class Player
{
public:
    Player(int id = 0);
    Player(int id, const char *name);
    Player(int id, std::string &name);

    const std::string& getName() const {return name;}

    int getMoney() const;
    void setMoney(int value);

    int getChips() const;
    void setChips(int value);

    Action getLastAction() const;
    void setLastAction(const Action &value);

    int getId() const;
    void setId(int value);

    int getSeat() const;
    void setSeat(int value);

    std::vector<Card> getHoleCards() const;
    void setHoleCards(const std::vector<Card> &value);

    int getWager() const;
    void setWager(int value);

private:
    int id;
    std::string name; //name of the player

    bool folded;
    bool allin;

    int seat; // 0 button, 1 small blind, 2 big blind, 3~7

    int money;
    int chips;

    //how much money this player has bet during the current game so far
    int wager;

    Action lastAction; //what the player did this turn (most recent action of this player)

    bool showdown; //if true, the hand card values of this player are stored in the holeCard variables.
    std::vector<Card> holeCards;

    // statistics
    unsigned int foldCount;
    unsigned int callCount;
    unsigned int raiseCount;
    unsigned int wins;
    unsigned int losses;
};

#endif // PLAYER_H
