/*
 * Bat.hpp
 *
 *  Created on: August 12th, 2020
 *      Author: rei de vries
 * Made for audio game jam, a bat flies around and tries to land on hills and
 * drink from water. When trees are implemented, will pollinate them.
 */

#ifndef SRC_COMPONENTS_BAT_H_
#define SRC_COMPONENTS_BAT_H_

struct Bat
{
	float stamina; //regain by resting on mountains, stamina=0 results in death
	float water; //regain by drinking, water=0 results in death
};

#endif
