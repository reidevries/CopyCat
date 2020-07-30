/*
 * WorldPos.h
 *
 *  Created on: Jul 17, 2020
 *      Author: rei de vries
 *      component for world position in 3D y-up
 */

#ifndef SRC_COMPONENTS_WORLDPOS_H_
#define SRC_COMPONENTS_WORLDPOS_H_

#include "CatConf.hpp"

struct WorldPos
{
	Vector3 pos;

	WorldPos() : pos({0,0,0}) {}
	WorldPos(Vector3 set_pos) : pos(set_pos) {}
};

#endif /* SRC_COMPONENTS_WORLDPOS_H_ */
