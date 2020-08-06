/*
 * Environment.hpp
 *
 *  Created on: Jun 9, 2020
 *      Author: rei de vries
 */

#ifndef SRC_ENVIRONMENT_H_
#define SRC_ENVIRONMENT_H_

#include <map>
#include <set>
#include <vector>
#include <string>
#include <cstdint>

#include <entt/entity/registry.hpp>
#include <entt/entity/fwd.hpp>

#include "CatConf.hpp"
#include "CatClock.hpp"
#include "IncludeComponents.hpp"
#include "DebugPrinter.hpp"
#include "MessageList.hpp"
#include "ManTex.hpp"
#include "ManAudio.hpp"
#include "Level.hpp"
#include "WorldConstants.hpp"

#ifndef CATCONF_H
#define CAT_VERBOSITY 0
#endif


class Environment
{
private:
	//number of grid squares to include in a bucket
	const int bucket_size;	//in world units
	const int world_size;	//in buckets
	entt::registry reg;

public:
	Environment(const int set_bucket_size, const int set_world_size)
		: bucket_size(set_bucket_size), world_size(set_world_size) {}
	Environment()
		: Environment(World::BUCKET_SIZE,World::METRE) {}

	entt::registry& getReg() {return reg;}

	void initLevel(ManTex& man_tex, 
		ManAudio& man_audio, 
		std::string level_name);

	void distributeMessages(MessageList messages);

	//updates all the gameobjects and accumulates their sent messages
	MessageList update(CatClock& clk, ManTex& man_tex);
};

#endif /* SRC_ENVIRONMENT_H_ */
