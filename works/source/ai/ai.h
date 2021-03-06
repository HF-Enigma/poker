#ifndef AI_H
#define AI_H

#include "engine/player.h"

class Game;

class AI : public Player
{
public:
    AI();
    AI(int id, const char *name);
    AI(int id, std::string &name);

    Action doTurn(Game *game);

    double getTightness() const;
    void setTightness(double value);

private:
    double tightness; // loosest 0 ~ 1.0 tightest
    int skill_level; // 0 beginner, 1 intermediate, 2 advanced

};

#endif // AI_H
