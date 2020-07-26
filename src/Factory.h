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
#include <array>
#include <sstream>
#include "ComponentIncludes.h"

class Factory
{
private:
	static int level_id_counter;

public:
	static entt::entity makeLevelObject(entt::registry& reg, std::string name);

	static entt::entity makePlane(entt::registry& reg,
		SpriteAnim sprite,
		Vector3 pos,
		Rectangle rect,
		VectorMath::Orthog dir);

	static entt::entity makeFloor(entt::registry& reg,
		SpriteAnim sprite,
		Vector3 pos,
		Rectangle rect);
};

#endif /* SRC_FACTORY_H_ */