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

#include "entt/entity/registry.hpp"
#include "gameobject.h"
#include "debugprinter.h"
#include "messagelist.h"
#include "resman.h"
#include "catclock.h"
#include "gameobject_list.h"

#include "catconf.h"
#ifndef CATCONF_H
#define CAT_VERBOSITY 0
#endif


class Environment
{
private:
	std::map<uint16_t, std::shared_ptr<GameObject>> object_buf;
	std::map<std::string, std::set<uint16_t>> objects_by_name;
	std::map<uint8_t, uint16_t> objects_by_render_order;
	//this vector always contains world_size x world_size sets,
	//but the sets may be empty
	std::vector<std::vector<std::set<uint16_t>>> objects_by_pos;

	//number of grid squares to include in a bucket
	const int bucket_size;	//in world units
	const int world_size;	//in buckets

	Level current_level;

	Vector2 getIndicesAtWorldPos(const Vector2 world_pos) const;
	std::map<int, std::shared_ptr<GameObject>> getObjectsAtBucket(
		int x, int y) const;

public:
	Environment();
	Environment(const int set_bucket_size, const int set_world_size);
	void insertObject(std::shared_ptr<GameObject> gameobject);
	void deleteObjectByID(const uint16_t id);

	//gets a pointer to a specific object
	std::shared_ptr<GameObject> getObjectByID(const uint16_t id) const;
	//gets the set of objects with the given name
	std::set<std::shared_ptr<GameObject>> getObjectsByName(
		const std::string name) const;
	//gets the set of objects within a box selection
	std::set<std::shared_ptr<GameObject>> getObjectsInBox(
		const Rectangle box) const;
	//gets a map of objects within a box selection, ordered by render distance
	std::multimap<int, std::shared_ptr<GameObject>> getObjectsInBoxForRender(
		const Rectangle box);
	//gets a vector of texsprites,
	//ordered by their owner gameobjects' render distance
	std::vector<TexSprite> getSpritesInBoxForRender(
		const Rectangle box);
	//gets the set of all objects within "radius" buckets of the coord
	//0 selects only the bucket the coord is within, higher values
	//include neighbours (including diagonals)
	std::set<std::shared_ptr<GameObject>> getObjectsByPos(
		const Vector2 coord, const int radius) const;
	std::set<std::shared_ptr<GameObject>> getAllObjects() const;


	void distributeMessages(MessageList messages);

	//updates all the gameobjects and accumulates their sent messages
	MessageList update(CatClock& clk, ResMan& resman);
};

#endif /* SRC_ENVIRONMENT_H_ */
