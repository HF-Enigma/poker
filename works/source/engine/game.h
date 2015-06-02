#ifndef GAME_H
#define GAME_H

#include <vector>

#include "player.h"

const unsigned int DEFAULT_BUFFER_SIZE = 1024*2;

enum Round
{
    ROUND_NONE,
    ROUND_PRE_FLOP,
    ROUND_FLOP,
    ROUND_TURN,
    ROUND_RIVER,
    ROUND_SHOWDOWN
};

class Player;
class AI;

class Game
{
public:
    Game(int gameSocket, int numOfPlayers = 8);
    ~Game();

    bool sendRegMsg(int playerID, const char *playerName, bool needNotify = false);
    bool sendActionMsg(const Action &a);

    // -1 for game over msg; 0 for unknown msg; 1 for sucess
    int  onMsg(char *msg, int size);

    void onSeatMsg(std::vector<char*> msg);
    void onGameOverMsg();
    void onBlindMsg(std::vector<char*> msg);
    void onHoleCardsMsg(std::vector<char*> msg);
    void onInquireMsg(std::vector<char*> msg);
    void onFlopMsg(std::vector<char*> msg);
    void onTurnMsg(std::vector<char*> msg);
    void onRiverMsg(std::vector<char*> msg);
    void onShowndownMsg(std::vector<char*> msg);
    void onPotWinMsg(std::vector<char*> msg);
    void onNotifyMsg(std::vector<char*> msg);

    Player *getPlayerById(int id);

    unsigned int getNumOfActivePlayers();

    int getSmallBlind() const;
    void setSmallBlind(int value);

    int getBigBlind() const;
    void setBigBlind(int value);

    int getPot() const;
    void setPot(int value);

    Round getRound() const;
    void setRound(const Round &value);

    std::vector<Card> getDeckCards() const;
    void setDeckCards(const std::vector<Card> &value);

    int getAmountToCall() const;

    int getMinToRaise() const;
    void setMinToRaise(int value);

private:
    int gameSocket;

    int smallBlind;
    int bigBlind;

    int pot;
    int amountToCall;
    int minToRaise;

    int button;

    Round round;

    std::vector<Card> deckCards;

    char *buffer, *buffer2, *buffer_end;

    int numOfPlayers;
    std::vector<Player*> players;
    AI *self;
};

#endif // GAME_H
