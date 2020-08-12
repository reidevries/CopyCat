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

void Systems::growOnPress(entt::registry& reg, 
	const CatClock& clk,
	const InputData::Mouse& mouse)
{
	//if something is being dragged, check for mouse movement or button release
	if (e_dragged.has_value()) {
		DroneSound& drone_sound = reg.get<DroneSound>(*e_dragged);
		SpriteQuad& sprite_quad = reg.get<SpriteQuad>(*e_dragged);
		
		sprite_quad.quad.p[0].y += mouse.ray_vel.position.y;
		sprite_quad.quad.p[1].y += mouse.ray_vel.position.y;
		float new_h = sprite_quad.quad.p[0].y-sprite_quad.quad.p[2].y;
		drone_sound.sound.vol = new_h/H_LIMIT;
		
		limitVolAndH(drone_sound, sprite_quad);
		
		if (mouse.l.rls || mouse.m.rls || mouse.r.rls) {
			e_dragged.reset();
			reg.get<SpriteAnim>(*e_dragged).sprite.tint = {
				255,
				255,
				255,
				255
			};
		}
	}
	
	
	//if mouse is not down, can't possibly be clicking on anything
	if (!mouse.l.down && !mouse.r.down && (mouse.scroll == 0)) return;
	//if mouse is not hovering over anything, 
	//can't possibly be clicking on anything
	if (!sound_on_hover_hit.has_value()) return;
	
	entt::entity e_hill = reg.get<Child>(*sound_on_hover_hit).e;
	DroneSound& drone_sound = reg.get<DroneSound>(e_hill);
	SpriteQuad& sprite_quad = reg.get<SpriteQuad>(e_hill);
	SpriteAnim& sprite_anim = reg.get<SpriteAnim>(e_hill);
	limitVolAndH(drone_sound, sprite_quad);
	
	float cur_vol = drone_sound.sound.vol;
	
	//change of height/volume is fastest when the height/volume is closest
	//to the maximum / 2
	//this is the function min(1, -32(x^2 - x))
	if (mouse.scroll != 0) {
		float delta = -32*(cur_vol*cur_vol - cur_vol);
		if (delta >= 1.0f) delta = 1.0f;
		delta = delta * GROWTH_RATE*clk.dt_s;
		float mod_h = delta*mouse.scroll;
		float mod_vol = delta*H_LIMIT*mouse.scroll;
		
		sprite_quad.quad.p[0].y += mod_h;
		sprite_quad.quad.p[1].y += mod_h;
		drone_sound.sound.vol += mod_vol;
	}
	
	if (mouse.l.press || mouse.r.press || mouse.m.press) {
		e_dragged = e_hill;
		sprite_anim.sprite.tint = {
			255,
			255,
			255,
			128
		};
	}
	
	float cur_h = sprite_quad.quad.p[0].y-sprite_quad.quad.p[2].y;
	
	//show water instead of hill when hill is at minimum size
	sprite_anim.show = (cur_h > 2);
	
	entt::entity e_water = reg.get<Child>(e_hill).e;
	SpriteAnim& water_sprite = reg.get<SpriteAnim>(e_water);
	water_sprite.show = (cur_h < 1);
}
