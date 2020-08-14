/*
 * Created: 08/08 2020
 *  by Rei de Vries
 */

#include "Systems.hpp"

using namespace std;
using namespace VectorMath;

static const float GROWTH_RATE = 4.5f;
static const int H_LIMIT = 48;

void limitVolAndH(DroneSound& drone_sound, SpriteQuad& sprite_quad) {
	drone_sound.sound.vol = limit(drone_sound.sound.vol);
	float cur_h = sprite_quad.quad.p[0].y - sprite_quad.quad.p[2].y;
	float cur_base_h = sprite_quad.quad.p[2].y;
	cur_h = limit(cur_h, H_LIMIT);
	sprite_quad.quad.p[0].y = cur_base_h + cur_h;
	sprite_quad.quad.p[1].y = cur_base_h + cur_h;
}

void deactivateHill(SpriteAnim& hill_sprite, std::size_t& num_active_hills) {
	hill_sprite.show = false;
	if (num_active_hills>0) --num_active_hills;
}

void activateHill(SpriteAnim& hill_sprite, std::size_t& num_active_hills) {
	hill_sprite.show = true;
	++num_active_hills;
}

void Systems::growOnPress(entt::registry& reg, 
	const CatClock& clk,
	const InputData::Mouse& mouse)
{
	//if something is being dragged, check for mouse movement or button release
	if (e_dragged.has_value()) {
		DroneSound& drone_sound = reg.get<DroneSound>(*e_dragged);
		SpriteQuad& sprite_quad = reg.get<SpriteQuad>(*e_dragged);
		SpriteAnim& sprite_anim = reg.get<SpriteAnim>(*e_dragged);
		
		sprite_quad.quad.p[0].y += mouse.ray_vel.position.y;
		sprite_quad.quad.p[1].y += mouse.ray_vel.position.y;
		float new_h = sprite_quad.quad.p[0].y-sprite_quad.quad.p[2].y;
		drone_sound.sound.vol = new_h/H_LIMIT;
		
		limitVolAndH(drone_sound, sprite_quad);
		
		if (mouse.l.rls || mouse.m.rls || mouse.r.rls
			|| (!mouse.l.down && !mouse.m.down && !mouse.r.down)) {
			e_dragged.reset();
			sprite_anim.sprite.tint = {
				255,
				255,
				255,
				255
			};
		} else {
			sprite_anim.sprite.tint = {
				230,
				230,
				255,
				128
			};
		}
	
		float cur_h = sprite_quad.quad.p[0].y-sprite_quad.quad.p[2].y;
		
		//if hill is reduced to minimum size, "deactivate" it
		if (sprite_anim.show && (cur_h <= 2)) {
			deactivateHill(sprite_anim, Systems::num_active_hills);
		} else if (!sprite_anim.show && (cur_h > 2)) {
			activateHill(sprite_anim, Systems::num_active_hills);
		}
		//if hill is reduced beyond minimum size, "activate" water instead
		entt::entity e_water = reg.get<Child>(*e_dragged).e;
		SpriteAnim& water_sprite = reg.get<SpriteAnim>(e_water);
		water_sprite.show = (cur_h < 1);
	} else { //if something innt being dragged, check for clicks
		//if mouse is not down, can't possibly be clicking on anything
		if (!mouse.l.down && !mouse.r.down && (mouse.scroll == 0)) return;
		//if mouse is not hovering over anything, 
		//can't possibly be clicking on anything
		if (!sound_on_hover_hit.has_value()) return;

		entt::entity e_hill = reg.get<Child>(*sound_on_hover_hit).e;
		if (mouse.l.press || mouse.r.press || mouse.m.press) {
			e_dragged = e_hill;
		}
	}
}
