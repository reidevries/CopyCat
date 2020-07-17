/*
 * environment.cpp
 *
 *  Created on: Jun 9, 2020
 *      Author: rei de vries
 */

#include "environment.h"

using namespace std;

Environment::Environment(const int set_bucket_size, const int set_world_size)
	: bucket_size(set_bucket_size), world_size(set_world_size)
{
	for (int x = 0; x < world_size; ++x) {
		vector<set<uint16_t>> column;
		for (int y = 0; y < world_size; ++y) {
			column.push_back(set<uint16_t>());
		}
		objects_by_pos.push_back(column);
	}
}

Environment::Environment()
	: Environment(8, 128)
{
}

Vector2 Environment::getIndicesAtWorldPos(const Vector2 world_pos) const
{
	int x_index = world_pos.x/bucket_size;
	int y_index = world_pos.y/bucket_size;
	if (x_index < world_size 	&& y_index < world_size
		&& x_index >= 0 			&& y_index >= 0) {
		return {static_cast<float>(x_index), static_cast<float>(y_index)};
	} else {
		return {-1, -1};
	}
}

map<int, shared_ptr<GameObject>> Environment::getObjectsAtBucket(
	int x,
	int y) const
{
	map<int, shared_ptr<GameObject>> accumulator;
	if (x >= 0 && x < world_size && y >= 0 && y < world_size) {
		for (auto const& id : objects_by_pos[x][y]) {
			accumulator.insert({id, object_buf.at(id)});
		}
	}
	return accumulator;
}

void Environment::insertObject(std::shared_ptr<GameObject> game_object)
{
	Vector2 bucket = getIndicesAtWorldPos(game_object->getPos());
	if (bucket.x < 0 || bucket.y < 0) {
		DebugPrinter::printDebug(0, "Environment::insertObject",
			"failed to insert" + game_object->name + " "
			+ to_string(game_object->id)
			+ " because position is outside environment boundary");
		return;
	}

	object_buf.insert({game_object->id, game_object});
	if (objects_by_name.count(game_object->name) > 0) {
		objects_by_name.at(game_object->name).insert(game_object->id);
	} else {
		set<uint16_t> id_set;
		id_set.insert(game_object->id);
		objects_by_name.insert({
			game_object->name,
			id_set
		});
	}

	objects_by_pos[bucket.x][bucket.y].insert(game_object->id);

	objects_by_render_order.insert({
		game_object->id,
		game_object->getRenderDistance()
	});

	DebugPrinter::printDebug(3, "Environment::insertObject",
		game_object->getInfo() + " successfully inserted into bucket at "
		+ VectorMath::printVector(bucket));

}

void Environment::deleteObjectByID(const uint16_t id)
{
	//before erasing object, erase its id from objects_by_name,
	//objects_by_render_order, and objects_by_pos
	shared_ptr<GameObject> cur_object = object_buf.at(id);
	string object_name = cur_object->name;
	objects_by_name.at(object_name).erase(id);
	uint8_t render_distance = cur_object->getRenderDistance();
	objects_by_render_order.erase(render_distance);
	Vector2 bucket = getIndicesAtWorldPos(cur_object->getPos());
	objects_by_pos.at(bucket.x).at(bucket.y).erase(id);

	object_buf.erase(id);
}

shared_ptr<GameObject> Environment::getObjectByID(const uint16_t id) const
{
	if (object_buf.count(id) > 0) return object_buf.at(id);
	else return nullptr;
}

set<shared_ptr<GameObject>> Environment::getObjectsByName(
	const string name) const
{
	set<shared_ptr<GameObject>> accumulator;
	if (objects_by_name.count(name) > 0) {
		for (auto const& id : objects_by_name.at(name)) {
			accumulator.insert(object_buf.at(id));
		}
	}
	return accumulator;
}

set<shared_ptr<GameObject>> Environment::getObjectsInBox(
	const Rectangle box) const
{
	Vector2 mins = getIndicesAtWorldPos({box.x, box.y});
	Vector2 maxs = getIndicesAtWorldPos({box.x+box.width, box.y+box.height});

	set<shared_ptr<GameObject>> accumulator;
	for (int u = mins.x; u <= maxs.x; ++u) {
		for (int v = mins.y; v <= maxs.y; ++v) {
			map<int, shared_ptr<GameObject>> local_objects
				= getObjectsAtBucket(u, v);
			for (auto const& object : local_objects) {
				accumulator.insert(object.second);
			}
		}
	}
	return accumulator;
}

multimap<int, shared_ptr<GameObject>> Environment::getObjectsInBoxForRender(
	const Rectangle box)
{
	Vector2 mins = getIndicesAtWorldPos({box.x, box.y});
	Vector2 maxs = getIndicesAtWorldPos({box.x+box.width, box.y+box.height});

	multimap<int, shared_ptr<GameObject>> accumulator;
	for (int u = mins.x; u <= maxs.x; ++u) {
		for (int v = mins.y; v <= maxs.y; ++v) {
			map<int, shared_ptr<GameObject>> local_objects
				= getObjectsAtBucket(u, v);
			for (auto& object : local_objects) {
				accumulator.insert({
					objects_by_render_order[object.first],
					object.second
				});
			}
		}
	}

	return accumulator;
}

vector<TexSprite> Environment::getSpritesInBoxForRender(const Rectangle box)
{
	Vector2 mins = getIndicesAtWorldPos({box.x, box.y});
	Vector2 maxs = getIndicesAtWorldPos({box.x+box.width, box.y+box.height});

	//put into a map to make sure it's sorted by render distance
	map<uint8_t, uint16_t> sorted_by_render_distance;
	for (int u = mins.x; u <= maxs.x; ++u) {
		for (int v = mins.y; v <= maxs.y; ++v) {
			for (auto const& object_id : objects_by_pos[u][v]) {
				sorted_by_render_distance.insert({
					object_buf.at(object_id)->getRenderDistance(),
					object_id
				});
			}
		}
	}


	vector<TexSprite> accumulator;
	//put tex sprites from objects into accumulator
	for (auto const& object_id : sorted_by_render_distance) {
		for (auto const& cur_sprite
			: object_buf.at(object_id.second)->getSprites()){
			accumulator.push_back(cur_sprite);
		}
	}
	return accumulator;
}

set<shared_ptr<GameObject>> Environment::getObjectsByPos(const Vector2 pos,
	const int radius) const
{
	Vector2 bucket = getIndicesAtWorldPos(pos);
	float diameter = static_cast<float>(radius*2);
	const Rectangle box = (Rectangle){
		bucket.x-radius,
		bucket.y-radius,
		diameter,
		diameter
	};
	return getObjectsInBox(box);
}

set<shared_ptr<GameObject>> Environment::getAllObjects() const {
	set<shared_ptr<GameObject>> accumulator;
	for (auto const& object : object_buf) {
		accumulator.insert(object.second);
	}
	return accumulator;
}


void Environment::distributeMessages(MessageList messages)
{
	for (auto& object : object_buf) {
		vector<Message> delivery = messages.getMessagesByDest(object.first);
		object.second->passMessages(delivery);
	}
}

MessageList Environment::update(CatClock& clk, ResMan& resman)
{
	MessageList temp;
	for (auto& object : object_buf) {
		vector<Message> new_messages = object.second->update(clk);
		temp.insert(new_messages);
	}
	return temp;
}
