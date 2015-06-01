#ifndef AI_H
#define AI_H

#include "engine/player.h"
#include "engine/game.h"

class AI : public Player
{
public:
    AI(std::string &name);

    Action doTurn(Game &game);

private:
    double tightness; // loosest 0 ~ 1.0 tightest
    int skill_level; // 0 beginner, 1 intermediate, 2 advanced

};

#endif // AI_H
