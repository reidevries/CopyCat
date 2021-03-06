/*
 * DroneSound.hpp
 *
 *  Created on: August 7th, 2020
 *      Author: rei de vries
 *		Component representing the a sound that drones continuously
 */

#ifndef SRC_COMPONENTS_DRONESOUND_H_
#define SRC_COMPONENTS_DRONESOUND_H_

#include <string>
#include <cstdint>

#include "../ResSound.hpp"
#include "../Fraction.hpp"

struct DroneSound 
{
	ResSound sound;
	Fraction ji;
};

#endif
