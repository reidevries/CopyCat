/*
 * environment.h
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

#include "CatConf.h"
#include "CatClock.h"
#include "ComponentIncludes.h"
#include "DebugPrinter.h"
#include "MessageList.h"
#include "ResMan.h"
#include "Level.h"
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
		: Environment(8,128) {}

	entt::registry& getReg() {return reg;}

	void initLevel(ResMan& res_man, std::string level_name);

	void distributeMessages(MessageList messages);

	//updates all the gameobjects and accumulates their sent messages
	MessageList update(CatClock& clk, ResMan& resman);
};

#endif /* SRC_ENVIRONMENT_H_ */
