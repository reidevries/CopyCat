/*
 * Factory.h
 *
 *  Created on: Jul 18, 2020
 *      Author: rei de vries
 */

#ifndef SRC_FACTORY_H_
#define SRC_FACTORY_H_

#include <entt/entity/registry.hpp>

#include "VectorMath.hpp"
#include "QuadDraw.hpp"
#include <array>
#include <string>
#include <sstream>
#include "IncludeComponents.hpp"
#include "WorldConstants.hpp"

class Factory
{
private:
	static int level_id_counter;

public:
	static entt::entity makeLevelObject(entt::registry& reg, 
		std::string name);
	
	static void setParentChild(entt::registry& reg,
		entt::entity child,
		entt::entity parent,
		std::string parent_level_obj,
		std::string child_level_obj);
	
	static void setParentChild(entt::registry& reg,
		entt::entity child,
		entt::entity parent);

	static entt::entity makePlane(entt::registry& reg,
		SpriteAnim sprite,
		Vector3 pos,
		Rectangle rect,
		VectorMath::Orthog dir);

	static entt::entity makeFloor(entt::registry& reg,
		SpriteAnim sprite,
		Vector3 pos);
	
	//for audio gamejam
	static entt::entity makeGrowThingy(entt::registry& reg,
		SpriteAnim hill_sprite,
		SpriteAnim water_sprite,
		ResSound drone_sound,
		entt::entity parent);
};

#endif /* SRC_FACTORY_H_ */
