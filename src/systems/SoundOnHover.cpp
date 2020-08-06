/*
 * Created: 02/08 2020
 *  by Rei de Vries
 */

#include "SoundOnHover.hpp"

using namespace std;

void Systems::soundOnHover(entt::registry& reg, 
	const InputData::Mouse& mouse, 
	const ResBuf<Sound, Res::AUDIO_BUF_SIZE>& audio_buf)
{
	auto view = reg.view<HoverSound, SpriteQuad, WorldPos>();
	
	for (const entt::entity e : view) {
		Vector3 e_pos = view.get<WorldPos>(e).pos;
		Quad sprite_quad = view.get<SpriteQuad>(e).quad;
		RayHitInfo ray_hit = sprite_quad.getRayHit(mouse.ray, e_pos);
		if (ray_hit.hit) {
			Sound s = view.get<HoverSound>(e).sound.getSound(audio_buf);
			PlaySound(s);
	
			stringstream ss;
			ss << " mouse ray at " 
				<< VectorMath::printVector(mouse.ray.position)
				<< " with direction "
				<< VectorMath::printVector(mouse.ray.direction) << endl;
			DebugPrinter::printDebug(4, "Systems::soundOnHover",  ss.str());
		}
	}
}
