#ifndef GAME_H
#define GAME_H

#include <vector>

#include "player.h"

enum Round
{
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

private:
    int gameSocket;

    int smallBlind;
    int bigBlind;

    int pot;

    int button;

    char *buffer, *buffer_start, *buffer_end;

    int numOfPlayers;
    std::vector<Player*> players;
    AI *self;
};

#endif // GAME_H
