/*
 * Child.hpp
 *
 *  Created on: August 8th, 2020
 *      Author: rei de vries
 *		component that links an entity to a child entity
 *		companion to Parent: should put Parent component in child entity
 */

#ifndef SRC_COMPONENTS_CHILD_H_
#define SRC_COMPONENTS_CHILD_H_

#include <entt/entity/registry.hpp>

struct Child 
{
	std::string name;
	entt::entity e;
};

#endif
