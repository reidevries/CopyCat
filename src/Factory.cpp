/*
 * Factory.cpp
 *
 *  Created on: Jul 18, 2020
 *      Author: rei de vries
 */

#include "Factory.h"

entt::entity ReiDV::makePlane(entt::registry& reg,
	SpriteAnim sprite,
	Vector3 pos,
	Rectangle rect,
	VectorMath::Orthog dir)
{
	const entt::entity e = reg.create();

	reg.emplace<SpriteAnim>(e, sprite);
	reg.emplace<WorldPos>(e, WorldPos(pos));

	SpriteQuad& quad = reg.emplace<SpriteQuad>(e);
	quad.quad = Quad(rect, dir);

	return e;
}

entt::entity ReiDV::makeFloor(entt::registry& reg,
	SpriteAnim sprite,
	Vector3 pos,
	Rectangle rect)
{
	return ReiDV::makePlane(reg, sprite, pos, rect, VectorMath::Orthog::up);
}
