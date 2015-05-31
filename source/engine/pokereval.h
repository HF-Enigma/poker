#ifndef POKEREVAL_H
#define POKEREVAL_H

#include <stdint.h>

namespace PokerEval
{

typedef uint64_t HandMask;

extern HandMask HandMasksTable[52];

typedef unsigned int HandVal;

void InitializeHandRankingTables(void);
extern HandVal RankHand(HandMask hand);

}

#endif // POKEREVAL_H
