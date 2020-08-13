/*
 * Created: 13/08 2020
 *  by Rei de Vries
 */

#include "Systems.hpp"

using namespace std;
using namespace VectorMath;

void Systems::velocity(entt::registry& reg,
		const CatClock& clk)
{
	auto view = reg.view<WorldPos, WorldVel>();
	for (const entt::entity e : view) {
		WorldPos& p = view.get<WorldPos>(e);
		WorldVel& v = view.get<WorldVel>(e);
		
		v.impulse = v.impulse * v.impulse_fall_rate;
		p.pos = p.pos + (v.vel + v.impulse)*clk.dt_s;
	}
}
