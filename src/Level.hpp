#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <memory>

#include <entt/entity/registry.hpp>
#include "Factory.h"
#include "ResMan.h"

namespace Level
{
	void loadTest(entt::registry& reg, ResMan& res_man);

	void load(entt::registry& reg, ResMan& res_man, std::string filename);
};

#endif
