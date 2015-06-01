#include "game.h"

#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

#include "player.h"

Game::Game(int gameSocket, int numOfPlayers) :
    gameSocket(gameSocket),
    numOfPlayers(numOfPlayers),
    players(numOfPlayers)
{
    for(size_t i=0; i<players.size(); ++i){
        players[i] = new Player();
    }
}

Game::~Game()
{
    for(size_t i=0; i<players.size(); ++i){
        delete players[i];
    }

    close(gameSocket);
}

bool Game::sendRegMsg(int playerID, const char *playerName, bool needNotify)
{
    char reg_msg[64];
    if(needNotify){
        snprintf(reg_msg, sizeof(reg_msg) - 1, "reg: %d %s need_notify \n", playerID, playerName);
    }
    else{
        snprintf(reg_msg, sizeof(reg_msg) - 1, "reg: %d %s \n", playerID, playerName);
    }
    return send(gameSocket, reg_msg, strlen(reg_msg) + 1, 0) != -1;
}

int Game::onMsg(char *msg, int size)
{
    return 0;
}
