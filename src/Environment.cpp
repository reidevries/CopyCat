/*
 * environment.cpp
 *
 *  Created on: Jun 9, 2020
 *      Author: rei de vries
 */

#include "Environment.h"

using namespace std;

void Environment::initLevel(ResMan& res_man, std::string level_name)
{
	Level::loadTest(reg, res_man);
}

