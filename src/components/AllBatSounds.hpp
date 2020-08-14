/*
 * AllBatSounds.hpp
 *
 *  Created on: Aug 14, 2020
 *      Author: rei de vries
 * stores every single bat sound
 */

#ifndef SRC_COMPONENTS_ALLBATSOUNDS_H_
#define SRC_COMPONENTS_ALLBATSOUNDS_H_

#include <array>

#include "CatConf.hpp"
#include "../WorldConstants.hpp"
#include "../ResSound.hpp"

struct AllBatSounds
{
	bool stub; //just to get it to work with my scripts please fix this
	std::array<std::array<ResSound, World::SIZE_Y>, World::SIZE_X> flute;
};

#endif /* SRC_COMPONENTS_ALLBATSOUNDS_H_ */
