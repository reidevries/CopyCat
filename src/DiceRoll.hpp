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
	static std::random_device rd;
	static std::mt19937::result_type seed;
};

#endif
