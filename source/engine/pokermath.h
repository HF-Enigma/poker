#ifndef POKERMATH_H
#define POKERMATH_H

/*
This source file contains various more complex mathematical poker functions.

Check combination.h, statistics.h, info.h, util.h and game.h for a bit more poker math functions!
*/

#include "card.h"
#include "combination.h"

double factorial(int i); //note: only works if result fits in double
double combination(int n, int p); //Binomial coefficient. Number of rows of p elements that can be made out of n elements, where order doesn't matter.

//starting hand analytics
bool isPair(const Card& card1, const Card& card2);
bool isSuited(const Card& card1, const Card& card2);
bool isConnector(const Card& card1, const Card& card2);

//pre-flop card rating: Group or EV

//gives a score (1-9) for starting hands, based on value and whether they're suited. Lower is better.
int getSklanskyMalmuthGroup(const Card& card1, const Card& card2);

//Card Expected Value statistics from http://www.pokerroom.com/
double getRealPlayStatisticsEV(const Card& card1, const Card& card2);

void splitIntoSuits(std::vector<Card>& spades
                  , std::vector<Card>& diamonds
                  , std::vector<Card>& hearts
                  , std::vector<Card>& clubs
                  , const std::vector<Card>& cards);

//does NOT reset the integers to 0 first, make sure you initialize them to 0!
void getAmountPerSuit(int& clubs, int& diamonds, int& hearts, int& spades, const std::vector<Card>& cards);

/*
The getPotEquity returns a value in the range 0.0-1.0 representing roughly how much win chance you have to win the pot,
depending on your hand cards and the known cards on the table. The opponents hands and unknown table cards can be ANY cards.
More precicely, it returns the percentage of money you'd win from the pot when you would be in this situation (=
these hand cards, these known table cards, and this amount of opponents) lots and lots of times. Sometimes you'll win,
sometimes you'll lose, in the end after so many plays you'll win that percentage of the pot.

If you want win, tie and lose chance instead of equity, use the getWinChance... functions below instead.

Getting pot equity is a type of poker math function that takes a long time to calculate and can be implemented
in many different ways with many different strategies (a combination of exhaustive search, monte carlo statistical
simulation, lookup tables and combinatorics). This one from OOPoker is just one possible implementation, and may be
changed or improved over time to become more accurate and more efficient. Its speed, accuracy and randomness
might change in future implementations.

Some inspiration for writing this function was gotten from:
http://www.codingthewheel.com/archives/multiway-ranged-isometric-equity-calculation-in-poker-1
http://www.pokerstove.com/

Parameters:
holeCards: vector must have size 2, represents your two hand cards
boardCards: vector must have size 0, 3, 4 or 5 (pre-flop, flop, turn, river), represents the known board cards
numOpponents: number of active opponents
numSamples: used when this function will use many samples to simulate many possible hand combinations. The higher the value, the more accurate the result, but the slower the function. 50000 is a good value.
*/
double getPotEquity(const std::vector<Card>& holeCards, const std::vector<Card>& boardCards, int numOpponents, int numSamples = 50000);


/*
This calculates the win chance of multiple players, all of which have known cards. This is useful
at pre-flop, flop or turn. At river there is no unknown factor anymore (but the function will still
return the correct result).

Parameters:
win, tie and lose: this are std::vectors that will have the same size as the number of players and contain the chances of each player.
holeCards1, holeCards2: the hand cards of all players. The size of these vectors must be the same and indicates the amount of players.
boardCards: 0 to 5 cards on the table.
numSamples: used when this function will use many samples to simulate many possible hand combinations. The higher the value, the more accurate the result, but the slower the function. 50000 is a good value.

returns false if error happened (such as invalid parameters)
*/
bool getWinChanceWithKnownHands(std::vector<double>& win, std::vector<double>& tie, std::vector<double>& lose
                              , const std::vector<Card>& holeCards1
                              , const std::vector<Card>& holeCards2
                              , const std::vector<Card>& boardCards
                              , int numSamples = 50000);



/*
This function evaluates combinations of 7 cards fast. This can be used for fast simulation.
The values of the return value aren't specified, except this: the higher the return value,
the better the combination. If the values are equal, it's a tie.
The input is cards as integers, and the integers must be created using eval7_index.
The integers are always positive.
*/
int eval7(const int* cards /*NOT card.getIndex()!!!*/);
int eval7_index(const Card& card);
ComboType eval7_category(int result); //converts result from eval to named combo type (without info about card values)

/*
Get win chance functions for Texas Hold'm.

They return the chance to win, tie or lose, as values in range 0.0-1.0.

The functions against 1 use exhaustive search.

The functions against N opponents use monte carlo statistical analysis.

You're encouraged to write your own win chance or win equity functions (using the eval7
function or a faster one for the hand evaluations). Check for example articles like here:
*/


void getWinChanceAgainst1AtFlop(double& win, double& tie, double& lose
                               , const Card& hand1, const Card& hand2
                               , const Card& table1, const Card& table2, const Card& table3);

void getWinChanceAgainst1AtTurn(double& win, double& tie, double& lose
                               , const Card& hand1, const Card& hand2
                               , const Card& table1, const Card& table2, const Card& table3, const Card& table4);

void getWinChanceAgainst1AtRiver(double& win, double& tie, double& lose
                               , const Card& hand1, const Card& hand2
                               , const Card& table1, const Card& table2, const Card& table3, const Card& table4, const Card& table5);


/*
The functions below calculate the win chance given your cards against N players.
Unlike the "against1" functions above, the ones below don't calculate the exact solution.
They use monte-carlo integration instead: you can choose an amount of random samples.

The higher the numSamples parameter, the more precise the solution, but the more calculation
time is needed. Setting it lower makes your bot faster.
*/

void getWinChanceAgainstNAtPreFlop(double& win, double& tie, double& lose
                                 , const Card& hand1, const Card& hand2
                                 , int numOpponents, int numSamples = 50000);


void getWinChanceAgainstNAtFlop(double& win, double& tie, double& lose
                               , const Card& hand1, const Card& hand2
                               , const Card& table1, const Card& table2, const Card& table3
                               , int numOpponents, int numSamples = 50000);

void getWinChanceAgainstNAtTurn(double& win, double& tie, double& lose
                               , const Card& hand1, const Card& hand2
                               , const Card& table1, const Card& table2, const Card& table3, const Card& table4
                               , int numOpponents, int numSamples = 50000);

void getWinChanceAgainstNAtRiver(double& win, double& tie, double& lose
                               , const Card& hand1, const Card& hand2
                               , const Card& table1, const Card& table2, const Card& table3, const Card& table4, const Card& table5
                               , int numOpponents, int numSamples = 50000);

#endif // POKERMATH_H
