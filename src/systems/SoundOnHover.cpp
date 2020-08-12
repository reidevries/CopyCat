/*
 * Created: 02/08 2020
 *  by Rei de Vries
 */

#include "Systems.hpp"

using namespace std;

void Systems::soundOnHover(entt::registry& reg, 
	const InputData::Mouse& mouse, 
	ManAudio& man_audio)
{
	//if mouse is not moving, there couldn't possibly be a new ray hit
	if (!mouse.moving) return;
	
	//first check the entity currently under the mouse to see if it is still
	//under the mouse. If so, don't bother to check other entities
	//this means only one entity can be under the mouse at once, which is fine
	//for audio game jam game
	if (sound_on_hover_hit.has_value()) {
		Vector3 e_pos = reg.get<WorldPos>(*sound_on_hover_hit).pos;
		Quad e_quad = reg.get<SpriteQuad>(*sound_on_hover_hit).quad;
		if (e_quad.getRayHit(mouse.ray, e_pos).hit) {
			return;
		} else {
			reg.get<SpriteAnim>(*sound_on_hover_hit).sprite.tint = {
				255,
				255,
				255,
				255
			};
			sound_on_hover_hit = std::nullopt;
		}
	}
	
	auto view = reg.view<HoverSound, SpriteQuad, WorldPos>();
	
	for (const entt::entity e : view) {
		Vector3 e_pos = view.get<WorldPos>(e).pos;
		Quad e_quad = view.get<SpriteQuad>(e).quad;
		if (e_quad.getRayHit(mouse.ray, e_pos).hit) {
			ResSound s = view.get<HoverSound>(e).sound;
			man_audio.playSound(s);
			sound_on_hover_hit = e;
			reg.get<SpriteAnim>(*sound_on_hover_hit).sprite.tint = {
				255,
				255,
				255,
				128
			};
			break; //only one hoversound can be hit by a ray at a time
		}
	}
}
