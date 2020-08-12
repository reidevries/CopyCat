/*
 * Parent.hpp
 *
 *  Created on: August 8th, 2020
 *      Author: rei de vries
 *		component that links an entity to a "parent" entity generically
 *		includes both name of the parent, and the entity id of the parent
 */

#ifndef SRC_COMPONENTS_PARENT_H_
#define SRC_COMPONENTS_PARENT_H_

#include <entt/entity/registry.hpp>

struct Parent 
{
	std::string name;
	entt::entity e;
};

#endif
