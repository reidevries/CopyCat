/*
 * Factory.cpp
 *
 *  Created on: Jul 18, 2020
 *      Author: rei de vries
 */

#include "Factory.hpp"

using namespace std;

int Factory::level_id_counter = 0;

entt::entity Factory::makeLevelObject(entt::registry& reg, string name)
{
	const entt::entity e = reg.create();
	stringstream ss;
	ss << setfill('0') << setw(4) << ++level_id_counter << "-" << name;
	reg.emplace<LevelID>(e, ss.str());
	return e;
}

entt::entity Factory::makePlane(entt::registry& reg,
	SpriteAnim sprite,
	Vector3 pos,
	Rectangle rect,
	VectorMath::Orthog dir)
{
	const entt::entity e = Factory::makeLevelObject(reg, "plane");

	reg.emplace<SpriteAnim>(e, sprite);
	reg.emplace<WorldPos>(e, WorldPos(pos));

	SpriteQuad& quad = reg.emplace<SpriteQuad>(e);
	quad.quad = Quad(rect, dir);

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
	quad.quad = Quad({0, 0, 32, 32}, VectorMath::Orthog::up);

	return e;
}
