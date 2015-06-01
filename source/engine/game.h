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

class Game
{
public:
    Game(int gameSocket, int numOfPlayers = 8);
    ~Game();

    bool sendRegMsg(int playerID, const char *playerName, bool needNotify = false);
    bool sendActionMsg(const Action &a);

    // -1 for game over msg; 0 for unknown msg; 1 for sucess
    int  onMsg(char *msg, int size);

    void onSeatMsg(char *msg);
    void onGameOverMsg();
    void onBlindMsg();
    void onHoleCardsMsg();
    void onInquireMsg();
    void onFlopMsg();
    void onTurnMsg();
    void onRiverMsg();
    void onShowndownMsg();
    void onPotWinMsg();
    void onNotifyMsg();

private:
    int gameSocket;
    int smallBlind;
    int bigBlind;

    int button;

    int numOfPlayers;
    std::vector<Player*> players;
};

#endif // GAME_H
