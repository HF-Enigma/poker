#include "ai.h"

#include <cstdio>

#include "engine/game.h"
#include "engine/pokermath.h"

double randomD(){
    return rand()/(double)RAND_MAX;
}

AI::AI() :
    Player()
{
}

AI::AI(int id, const char *name) :
    Player(id, name)
{
}

AI::AI(int id, std::string &name) :
    Player(id, name)
{
}

Action AI::doTurn(Game *game)
{
    printf("AI thinking ................................\n");
    double raiseChance = 0.0;
    double allInChance = 0.0;
    int maxWager = 0;

    int blind = game->getSmallBlind();

    Round r = game->getRound();

    if(r == ROUND_NONE){
        fprintf(stderr, "Invalid round, some thing is wrong\n");
    }
    else if(r == ROUND_PRE_FLOP)
    {
        printf("AI getSklanskyMalmuthGroup ................................\n");
        int group = getSklanskyMalmuthGroup(holeCards[0], holeCards[1]);
        if(group == 9) maxWager = 0;
        else if(group == 8 || group == 7) maxWager = blind*2;
        else if(group == 6 || group == 5)
        {
            maxWager = blind * 4;
            raiseChance = 0.2;
        }
        else if(group == 4 || group == 3)
        {
            maxWager = this->chips;
            raiseChance = 0.5;
            allInChance = 0.1;
        }
        else if(group == 2 || group == 1)
        {
            maxWager = this->chips;
            raiseChance = 0.5;
            allInChance = 0.2;
        }
    }
    else
    {
        double win = 0.0, tie = 0.0, lose = 0.0;
        int num_opponents = game->getNumOfActivePlayers() - 1;

        printf("AI getWinChance ................................\n");
        if(r == ROUND_FLOP){
            if(game->getDeckCards().size() != 3){
                fprintf(stderr, "Invalid deck, some thing is wrong\n");
            }
            getWinChanceAgainstNAtFlop(win, tie, lose, holeCards, game->getDeckCards(), num_opponents);
        }
        else if(r == ROUND_TURN){
            if(game->getDeckCards().size() != 4){
                fprintf(stderr, "Invalid deck, some thing is wrong\n");
            }
            getWinChanceAgainstNAtTurn(win, tie, lose, holeCards, game->getDeckCards(), num_opponents);
        }
        else if(r == ROUND_RIVER){
            if(game->getDeckCards().size() != 5){
                fprintf(stderr, "Invalid deck, some thing is wrong\n");
            }
            getWinChanceAgainstNAtRiver(win, tie, lose, holeCards, game->getDeckCards(), num_opponents);
        }

        if(win > tightness)
        {
            maxWager = this->chips;
            raiseChance = 0.5;
            allInChance = 0.5;
        }
        else if(win > tightness / 2)
        {
            maxWager = (this->chips) / 3;
            raiseChance = 0.2;
        }
        else if(tie > 0.8)
        {
            //large chance to tie (maybe there's a royal flush on the table or so). Never fold in this case!
            raiseChance = 0;
            allInChance = 0;
            maxWager = this->chips;
        }
        else //misery... only check or fold
        {
            maxWager = 0;
        }
    }

    printf("AI action ................................\n");

    Action a;
    a.command = ACTION_FOLD;

    int minWagerTotal = game->getAmountToCall();

    if(minWagerTotal > this->chips){
        minWagerTotal = this->chips;
    }

    if(minWagerTotal > maxWager) {
        a.command = ACTION_FOLD;
    }
    else {
        if(randomD() < raiseChance) {
            int amount = (int)(game->getMinToRaise() * (1.0 + randomD()));

            //round the amount to become a multiple of small blinds
            if(blind == 0){
                printf("SmallBilind is 0\n");
            }
            else{
                amount = (amount / blind) * blind;
                if(amount == 0) amount = game->getMinToRaise();
            }

            a.command = ACTION_RAISE;
            a.amount = amount;
        }
        else if(randomD() < allInChance) {
            a.command = ACTION_ALLIN;
        }
        else {
            a.command = ACTION_CALL;
        }
    }

    printf("AI done ................................\n");

    return a;
}

double AI::getTightness() const
{
    return tightness;
}

void AI::setTightness(double value)
{
    tightness = value;
}


