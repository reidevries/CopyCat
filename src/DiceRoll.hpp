/*
 * Created: 12/08 2020
 *  by Rei de Vries
 */
 
#ifndef SRC_DICEROLL_H_
#define SRC_DICEROLL_H_

#include <chrono>
#include <random>

class DiceRoll 
{
private:
	std::random_device rd;
	std::mt19937::result_type seed;
	std::mt19937 gen;
	
public:
	DiceRoll();
	
	//returns a float between 0.0 and 1.0
	float rollF();
};

#endif
