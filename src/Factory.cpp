/*
 * Factory.cpp
 *
 *  Created on: Jul 18, 2020
 *      Author: rei de vries
 */

#include "Factory.hpp"

using namespace std;
using namespace VectorMath;

int Factory::level_id_counter = 0;

entt::entity Factory::makeLevelObject(entt::registry& reg, string name)
{
	const entt::entity e = reg.create();
	stringstream ss;
	ss << setfill('0') << setw(4) << ++level_id_counter << "-" << name;
	reg.emplace<LevelObj>(e, ss.str());
	return e;
}

void Factory::setParentChild(entt::registry& reg,
	entt::entity child,
	entt::entity parent,
	string parent_level_obj,
	string child_level_obj)
{
	reg.emplace<Parent>(child, parent_level_obj, parent);
	reg.emplace<Child>(parent, child_level_obj, child);
}

void Factory::setParentChild(entt::registry& reg,
	entt::entity child,
	const entt::entity parent)
{
	string parent_name = to_string(static_cast<int>(parent));
	//check if the parent has a LevelObj name
	if (reg.has<LevelObj>(parent)) {
		parent_name = reg.get<LevelObj>(parent).name;
	}
	string child_name = to_string(static_cast<int>(child));
	//check if the parent has a LevelObj name
	if (reg.has<LevelObj>(child)) {
		child_name = reg.get<LevelObj>(child).name;
	}
	setParentChild(reg, child, parent, parent_name, child_name);
}


entt::entity Factory::makePlane(entt::registry& reg,
	SpriteAnim sprite,
	Vector3 pos,
	Rectangle rect,
	Orthog dir)
{
	const entt::entity e = Factory::makeLevelObject(reg, "plane");

	reg.emplace<SpriteAnim>(e, sprite);
	reg.emplace<WorldPos>(e, WorldPos(pos));

	SpriteQuad& quad = reg.emplace<SpriteQuad>(e);
	quad.quad = Quad(rect, dir);
	reg.emplace<HitBox>(e, quad.quad.getBoundingBox(pos));

	return e;
}

entt::entity Factory::makeFloor(entt::registry& reg,
	SpriteAnim sprite,
	Vector3 pos)
{
	const entt::entity e = Factory::makeLevelObject(reg, "floor");

	reg.emplace<SpriteAnim>(e, sprite);
	reg.emplace<WorldPos>(e, WorldPos(pos));

	SpriteQuad& quad = reg.emplace<SpriteQuad>(e);
	quad.quad = Quad({0, 0, World::METRE, World::METRE}, 
		Orthog::up);
	reg.emplace<HitBox>(e, quad.quad.getBoundingBox(pos));

	return e;
}

entt::entity Factory::makeGrowThingy(entt::registry& reg,
		SpriteAnim hill_sprite,
		SpriteAnim water_sprite,
		ResSound drone_sound,
		entt::entity parent)
{
	entt::entity e_hill = Factory::makeLevelObject(reg, "growthingy");
	Factory::setParentChild(reg, e_hill, parent);
	
	WorldPos parent_pos = reg.get<WorldPos>(parent);
	Vector3 new_pos = parent_pos.pos + Vector3{0,0.1f,0};
	
	hill_sprite.show = false;
	water_sprite.show = false;
	water_sprite.anim_speed = 4.20f;
	
	reg.emplace<SpriteAnim>(e_hill, hill_sprite);
	reg.emplace<WorldPos>(e_hill, new_pos);
	SpriteQuad& quad = reg.emplace<SpriteQuad>(e_hill);
	quad.quad = Quad({0, 0, World::METRE, 1}, Orthog::facing_cam);
	drone_sound.vol = 0.0f;
	DroneSound& s = reg.emplace<DroneSound>(e_hill, drone_sound);
	s.ji = reg.get<HoverSound>(parent).ji;
	reg.emplace<Hill>(e_hill);
	
	entt::entity e_water = Factory::makeFloor(
		reg, 
		water_sprite,
		new_pos);
	reg.emplace<Water>(e_water);
	Factory::setParentChild(reg, e_water, e_hill);
	
	return e_hill;
}

entt::entity Factory::makeBat(entt::registry& reg,
	Vector3 pos,
	SpriteAnim bat_sprite)
{
	entt::entity e_bat = reg.create();
	reg.emplace<WorldPos>(e_bat, pos);
	reg.emplace<WorldVel>(e_bat, Vector3{0,0,0}, Vector3{0,0,0});
	reg.emplace<SpriteAnim>(e_bat, bat_sprite);
	Quad q = Quad(
		{0,0, World::METRE*0.4f, World::METRE*0.5f}, 
		Orthog::facing_cam
	);
	reg.emplace<SpriteQuad>(e_bat, q);
	// initialize the bat with full stamina and full water needs
	Bat& b = reg.emplace<Bat>(e_bat, 1.0f, 1.0f); 
	b.state = Bat::State::flying;
	return e_bat;
}
