#include "ai.h"

#include "engine/game.h"
#include "engine/pokermath.h"

double randomD(){
    return rand()/(double)RAND_MAX;
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
    double raiseChance = 0.0;
    double allInChance = 0.0;
    int maxWager = 0;

    int bb = game->getBigBlind();

    Round r = game->getRound();

    if(r == ROUND_PRE_FLOP)
    {
        int group = getSklanskyMalmuthGroup(holeCards[0], holeCards[1]);
        if(group == 9) maxWager = 0;
        else if(group == 8 || group == 7) maxWager = bb;
        else if(group == 6 || group == 5)
        {
            maxWager = bb * 4;
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
        double win, tie, lose;
        int num_opponents = game->getNumOfActivePlayers() - 1;
        if(r == ROUND_FLOP){
            getWinChanceAgainstNAtFlop(win, tie, lose, holeCards, game->getDeckCards(), num_opponents);
        }
        else if(r == ROUND_TURN){
            getWinChanceAgainstNAtTurn(win, tie, lose, holeCards, game->getDeckCards(), num_opponents);
        }
        else if(r == ROUND_RIVER){
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

    Action a;

    int minWagerTotal = game->getAmountToCall();
    if(minWagerTotal > this->chips) minWagerTotal = this->chips;

    if(minWagerTotal > maxWager)
    {
        a.command = ACTION_FOLD;
        return a;
    }
    else
    {
        if(randomD() < raiseChance)
        {
            int amount = (int)(game->getMinToRaise() * (1.0 + randomD()));

            //round the amount to become a multiple of big blinds
            amount = (amount / bb) * bb;
            if(amount == 0) amount = game->getMinToRaise();

            a.command = ACTION_RAISE;
            a.amount = amount;
            return a;
        }
        else if(randomD() < allInChance)
        {
            a.command = ACTION_ALLIN;
            return a;
        }
        else
        {
            a.command = ACTION_CALL;
            return a;
        }
    }

    a.command = ACTION_FOLD;
    return a;
}

