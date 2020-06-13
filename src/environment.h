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
#include <raylib.h>
#include <string>
#include "gameobject.h"
#include "debugprinter.h"
#include "messagelist.h"
#include "resman.h"
#include "gameobject_list.h"

#include "catconf.h"
#ifndef CATCONF_H
#define CAT_VERBOSITY 0
#endif


class Environment
{
private:
	std::map<int, std::shared_ptr<GameObject>> object_buf;
	std::map<std::string, std::set<int>> objects_by_name;
	std::map<int, int> objects_by_zpos;

	//number of grid squares to include in a bucket
	const int bucket_size;	//in world units
	const int world_size;	//in buckets
	//this vector always contains world_size x world_size sets,
	//but the sets may be empty
	std::vector<std::vector<std::set<int>>> objects_by_pos;

	Level current_level;

	Vector2 getIndicesAtWorldPos(const Vector2 world_pos);
	std::map<int, std::shared_ptr<GameObject>> getObjectsAtBucket(
		int x, int y);

public:
	Environment();
	Environment(const int set_bucket_size, const int set_world_size);
	void insertObject(std::shared_ptr<GameObject> gameobject);
	void deleteObjectByID(const int id);

	//gets a pointer to a specific object
	std::shared_ptr<GameObject> getObjectByID(const int id);
	//gets the set of objects with the given name
	std::set<std::shared_ptr<GameObject>> getObjectsByName(const std::string name);
	//gets the set of objects within a box selection
	std::set<std::shared_ptr<GameObject>> getObjectsInBox(const Rectangle box);
	//gets a map of objects within a box selection, ordered by render distance
	std::map<int, std::shared_ptr<GameObject>> getObjectsInBoxForRender(
		const Rectangle box);
	//gets the set of all objects within "radius" buckets of the coord
	//0 selects only the bucket the coord is within, higher values
	//include neighbours (including diagonals)
	std::set<std::shared_ptr<GameObject>> getObjectsByPos(
		const Vector2 coord, const int radius);
	std::set<std::shared_ptr<GameObject>> getAllObjects();


	void distributeMessages(MessageList messages);

	//updates all the gameobjects and accumulates their sent messages
	MessageList update(const float dt,
		const unsigned int time_s,
		const unsigned int tick,
		ResMan& resman);
};

#endif /* SRC_ENVIRONMENT_H_ */
