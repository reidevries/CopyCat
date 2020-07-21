/*
 * Factory.cpp
 *
 *  Created on: Jul 18, 2020
 *      Author: rei de vries
 */

#include "Factory.h"

entt::entity Cat::makePlane(entt::registry& reg,
	SpriteAnim sprite,
	Vector3 pos,
	Rectangle rect,
	Cat::Orthog dir)
{
	const entt::entity e = reg.create();

	reg.emplace<SpriteAnim>(e, sprite);
	reg.emplace<WorldPos>(e, WorldPos(pos));

	SpriteQuad& quad = reg.emplace<SpriteQuad>(e);
	quad.quad = Quad(rect, dir);

	return e;
}

entt::entity Cat::makeFloor(entt::registry& reg,
	SpriteAnim sprite,
	Vector3 pos,
	Rectangle rect)
{
	return Cat::makePlane(reg, sprite, pos, rect, Cat::Orthog::up);
}
