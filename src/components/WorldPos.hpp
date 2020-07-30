/*
 * WorldPos.h
 *
 *  Created on: Jul 17, 2020
 *      Author: rei de vries
 *      component for world position in 3D y-up
 */

#ifndef SRC_COMPONENTS_WORLDPOS_H_
#define SRC_COMPONENTS_WORLDPOS_H_

#include "CatConf.h"

struct WorldPos
{
	Vector3 pos;

	WorldPos(Vector3 set_pos) : pos(set_pos) {}
};

#endif /* SRC_COMPONENTS_WORLDPOS_H_ */
