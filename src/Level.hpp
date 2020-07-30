#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <memory>

#include <entt/entity/registry.hpp>
#include "Factory.hpp"
#include "ResMan.hpp"

namespace Level
{
	void loadTest(entt::registry& reg, ResMan& res_man);

	void load(entt::registry& reg, ResMan& res_man, std::string filename);
};

#endif
