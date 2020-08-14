/*
 * WorldVel.hpp
 *
 *  Created on: Aug 13, 2020
 *      Author: rei de vries
 *      component for world velocity in 3d, corresponding to WorldPos
 */

#ifndef SRC_COMPONENTS_WORLDVEL_H_
#define SRC_COMPONENTS_WORLDVEL_H_

#include "CatConf.hpp"

struct WorldVel
{
	Vector3 vel;
	//set this impulse value, and the velocity will set by velocity system
	//according to the impulse direction and magnitude
	Vector3 impulse; 
	//determines how fast the impulse falls off
	float impulse_fall_rate = 0.5f;
};

#endif /* SRC_COMPONENTS_WORLDVEL_H_ */
