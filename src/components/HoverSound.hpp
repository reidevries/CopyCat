/*
 * HoverSound.hpp
 *
 *  Created on: August 2nd, 2020
 *      Author: rei de vries
 *		Component representing a sound that plays when the mouse hovers over
 */

#ifndef SRC_COMPONENTS_HOVERSOUND_H_
#define SRC_COMPONENTS_HOVERSOUND_H_

#include <string>
#include <cstdint>

#include "../ResSound.hpp"
#include "../Fraction.hpp"

struct HoverSound 
{
	ResSound sound;
	Fraction ji;
};

#endif
