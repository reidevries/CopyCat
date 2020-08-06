/*
 * environment.cpp
 *
 *  Created on: Jun 9, 2020
 *      Author: rei de vries
 */

#include "Environment.hpp"

using namespace std;

void Environment::initLevel(ManTex& man_tex, 
	ManAudio& man_audio, 
	string level_name)
{
	Level::loadTest(reg, man_tex,  man_audio);
}

