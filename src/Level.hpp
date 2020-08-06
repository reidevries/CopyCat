#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <memory>

#include <entt/entity/registry.hpp>
#include "Factory.hpp"
#include "ManTex.hpp"
#include "ManAudio.hpp"

namespace Level
{
	void loadTest(entt::registry& reg, ManTex& man_tex,  ManAudio& man_audio);

	void load(entt::registry& reg, ManTex& man_tex, std::string filename);
};

#endif
