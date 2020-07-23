/*
 * Factory.h
 *
 *  Created on: Jul 18, 2020
 *      Author: rei de vries
 */

#ifndef SRC_FACTORY_H_
#define SRC_FACTORY_H_

#include <entt/entity/registry.hpp>

#include "VectorMath.h"
#include "QuadDraw.h"
#include "ComponentList.h"
#include <array>

namespace ReiDV
{
entt::entity makePlane(entt::registry& reg,
	SpriteAnim sprite,
	Vector3 pos,
	Rectangle rect,
	VectorMath::Orthog dir);

entt::entity makeFloor(entt::registry& reg,
	SpriteAnim sprite,
	Vector3 pos,
	Rectangle rect);
};

#endif /* SRC_FACTORY_H_ */
