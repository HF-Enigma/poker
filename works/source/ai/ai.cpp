#include "ai.h"

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
    Action a;
    a.command = ACTION_FOLD;
    return a;
}

