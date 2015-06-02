#include "game.h"

#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <ctime>

#include <sys/socket.h>
#include <unistd.h>

#include "player.h"
#include "ai/ai.h"

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

const char MSG_FUNCS[][32] = {
    "onSeatMSg()",
    "onBlindMSg()",
    "onHoleMSg()",
    "onInquireMSg()",
    "onFlopMSg()",
    "onTurnMSg()",
    "onRiverMSg()",
    "onShowdownMSg()",
    "onPotWinMSg()",
    "onNotifyMSg()",
};


Game::Game(int gameSocket, int numOfPlayers) :
    gameSocket(gameSocket),
    amountToCall(0),
    numOfPlayers(numOfPlayers),
    self(NULL)
{
    players.reserve(numOfPlayers);

    buffer = new char[DEFAULT_BUFFER_SIZE];
    buffer2 = new char[DEFAULT_BUFFER_SIZE];
    memset(buffer, 0, DEFAULT_BUFFER_SIZE);
    buffer_end = buffer;
}

Game::~Game()
{
    for(size_t i=0; i<players.size(); ++i){
        delete players[i];
    }

    close(gameSocket);

    delete[] buffer;
    delete[] buffer2;
}

bool Game::sendRegMsg(int playerID, const char *playerName, bool needNotify)
{
    self = new AI(playerID, playerName);
    players.push_back(self);

    char msg[64];
    if(needNotify){
        snprintf(msg, sizeof(msg) - 1, "reg: %d %s need_notify \n", playerID, playerName);
    }
    else{
        snprintf(msg, sizeof(msg) - 1, "reg: %d %s \n", playerID, playerName);
    }
    return send(gameSocket, msg, strlen(msg) + 1, 0) != -1;
}

bool Game::sendActionMsg(const Action &a)
{
    char msg[64];

    switch(a.command){
    case ACTION_RAISE:
        snprintf(msg, sizeof(msg), "raise %d \n", a.amount);
        break;
    case ACTION_FOLD:
        snprintf(msg, sizeof(msg), "fold \n");
        break;
    case ACTION_CHECK:
        snprintf(msg, sizeof(msg), "check \n");
        break;
    case ACTION_CALL:
        snprintf(msg, sizeof(msg), "call \n");
        break;
    case ACTION_ALLIN:
        snprintf(msg, sizeof(msg), "all_in \n");
        break;
    default:
        break;
    }

    return send(gameSocket, msg, strlen(msg) + 1, 0) != -1;
}

int Game::onMsg(char *msg, int size)
{
    //
    memcpy(buffer_end, msg , size);
    buffer_end += size;

    // split message
    std::vector<char*> strs;
    char *buffer_start = buffer;

    char *pch;
    while(buffer_start < buffer_end){
        pch = strpbrk(buffer_start, "\n");
        if(pch && pch < buffer_end){
            *pch = '\0';
            strs.push_back(buffer_start);
            printf("%s\n", buffer_start);
            buffer_start = pch+1;
        }
        else{
            break;
        }
    }

    printf("--------------------------------------------------\n");

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
                        clock_t t = clock();
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
                        t = clock() - t;
                        printf("-------- Time to call %s : %f s -------\n", MSG_FUNCS[i], t/(float)CLOCKS_PER_SEC);
                    }
                }
                break;
            }
        }
    }

    if(buffer_start >= buffer_end){
        memset(buffer, 0, DEFAULT_BUFFER_SIZE);
        buffer_start = buffer;
        buffer_end = buffer;
    }
    else{
        memcpy(buffer2, buffer_start, (buffer_end-buffer_start));
        memset(buffer, 0, DEFAULT_BUFFER_SIZE);
        memcpy(buffer, buffer2, (buffer_end-buffer_start));
        buffer_end = buffer + (buffer_end-buffer_start);
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

    int pot= 0;
    if(sscanf(msg[msg.size()-1], "total pot: %d", &pot) == 1){
        this->pot = pot;
    }

    Action a = self->doTurn(this);
    sendActionMsg(a);
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

unsigned int Game::getNumOfActivePlayers()
{
    unsigned int c = 0;

    for(size_t i=0; i<players.size(); ++i){
        if(players[i]->isActive()){
            ++c;
        }
    }
    return c;
}

int Game::getSmallBlind() const
{
    return smallBlind;
}

void Game::setSmallBlind(int value)
{
    smallBlind = value;
}

int Game::getBigBlind() const
{
    return bigBlind;
}

void Game::setBigBlind(int value)
{
    bigBlind = value;
}

int Game::getPot() const
{
    return pot;
}

void Game::setPot(int value)
{
    pot = value;
}

Round Game::getRound() const
{
    return round;
}

void Game::setRound(const Round &value)
{
    round = value;
}

std::vector<Card> Game::getDeckCards() const
{
    return deckCards;
}

void Game::setDeckCards(const std::vector<Card> &value)
{
    deckCards = value;
}

int Game::getAmountToCall() const
{
    return amountToCall;
}

int Game::getMinToRaise() const
{
    return minToRaise;
}

void Game::setMinToRaise(int value)
{
    minToRaise = value;
}
