#include "game.h"

#include <string>
#include <cstring>
#include <cstdio>
#include <sys/socket.h>
#include <unistd.h>

#include "player.h"

const unsigned int DEFAULT_BUFFER_SIZE = 1024*4;

const char MSG_TAGS[10][2][16] = {
    {"seat/ ", "/seat "},
    {"blind/ ", "/blind "},
    {"hold/ ", "/hold "},
    {"inquire/ ", "/inquire "},
    {"flop/ ", "/flop "},
    {"turn/ ", "/turn "},
    {"river/ ", "/river "},
    {"showdown/ ", "/showdown "},
    {"pot-win/ ", "/pot-win "},
    {"notify/ ", "/notify "},
};

Game::Game(int gameSocket, int numOfPlayers) :
    gameSocket(gameSocket),
    numOfPlayers(numOfPlayers),
    self(NULL)
{
    players.reserve(numOfPlayers);

    buffer = new char[DEFAULT_BUFFER_SIZE];
    memset(buffer, 0, DEFAULT_BUFFER_SIZE);
    buffer_start = buffer;
    buffer_end = buffer;
}

Game::~Game()
{
    if(self){
        delete self;
    }

    for(size_t i=0; i<players.size(); ++i){
        delete players[i];
    }

    close(gameSocket);

    delete[] buffer;
}

bool Game::sendRegMsg(int playerID, const char *playerName, bool needNotify)
{
    self = new Player(playerID, playerName);
    players.push_back(self);

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
    //
    memcpy(buffer_end+1, msg , size);
    buffer_end += size;

    // split message
    std::vector<char*> strs;
    buffer_start = buffer;

    int len = 0;
    while(buffer_start <= buffer_end){
        len = strlen(buffer_start);
        if(len>0 && (buffer_start + (len-1)) <= buffer_end){
            strs.push_back(buffer_start);
            buffer_start += len+1;
        }
        else{
            break;
        }
    }

    // handling messages
    while(strs.size() > 0){
        if(strcmp(strs[0], "game-over ") == 0){ // game-over-msg
            onGameOverMsg();
            return -1;
        }

        // match message tags
        for(int i=0; i<10; ++i){
            if(strcmp(strs[0], MSG_TAGS[i][0]) == 0){
                for(size_t j=1; j< strs.size(); ++j){
                    if(strcmp(strs[j], MSG_TAGS[i][1]) == 0){
                        std::vector<char*> msgs(strs.begin(), strs.begin()+j+1);
                        strs.erase(strs.begin(), strs.begin()+j+1);
                        switch(i){
                        case 0: onSeatMsg(msgs); break;
                        case 1: onBlindMsg(msgs); break;
                        case 2: onHoleCardsMsg(msgs); break;
                        case 3: onInquireMsg(msgs); break;
                        case 4: onFlopMsg(msgs); break;
                        case 5: onTurnMsg(msgs); break;
                        case 6: onRiverMsg(msgs); break;
                        case 7: onShowndownMsg(msgs); break;
                        case 8: onPotWinMsg(msgs); break;
                        case 9: onNotifyMsg(msgs); break;
                        }
                    }
                }
                break;
            }
        }
    }

    return 0;
}

void Game::onSeatMsg(std::vector<char *> msg)
{
    int pid, jetton, money;
    Player *p;

    for (size_t i=1; i<msg.size()-1; ++i) {
        if(sscanf(msg[i], "%*[^0-9]%d %d %d", &pid, &jetton, &money) == 3){
            p = getPlayerById(pid);
            p->setSeat(i-1);
            p->setChips(jetton);
            p->setMoney(money);
        }
    }
}

void Game::onGameOverMsg()
{
}

void Game::onBlindMsg(std::vector<char *> msg)
{
    int pid, bet;
    Player *p;

    for (size_t i=1; i<msg.size()-1; ++i) {
        if(sscanf(msg[i], "%d: %d ", &pid, &bet) == 2){
            p = getPlayerById(pid);
            p->setChips(p->getChips() - bet);
        }
    }
}

void Game::onHoleCardsMsg(std::vector<char *> msg)
{
    char color[16], point[4];

    std::vector<Card> holeCards;
    for (size_t i=1; i<msg.size(); ++i) {
        if(sscanf(msg[i], "%s %s ", color, point) == 2){
            holeCards.push_back(Card(color, point));
        }
    }
    self->setHoleCards(holeCards);
}

void Game::onInquireMsg(std::vector<char *> msg)
{
    int pid, jetton, money, bet;
    char action[8];
    Player *p;

    for (size_t i=1; i<msg.size()-2; ++i) {
        if(sscanf(msg[i], "%d %d %d %d %s", &pid, &jetton, &money, &bet, action) == 5){
            p = getPlayerById(pid);
            p->setChips(jetton);
            p->setMoney(money);
            p->setWager(bet);
        }
    }
}

void Game::onFlopMsg(std::vector<char *> msg)
{

}

void Game::onTurnMsg(std::vector<char *> msg)
{

}

void Game::onRiverMsg(std::vector<char *> msg)
{

}

void Game::onShowndownMsg(std::vector<char *> msg)
{

}

void Game::onPotWinMsg(std::vector<char *> msg)
{

}

void Game::onNotifyMsg(std::vector<char *> msg)
{

}

Player *Game::getPlayerById(int id)
{
    for(size_t i=0; i<players.size(); ++i){
        if(players[i]->getId() == id){
            return players[i];
        }
    }

    Player *p = new Player(id);
    players.push_back(p);
    return p;
}
