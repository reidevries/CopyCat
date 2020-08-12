/*
 * Created: 08/08 2020
 *  by Rei de Vries
 */

#include "Systems.hpp"

using namespace std;

static const float GROWTH_RATE = 0.1f;

void Systems::growOnPress(entt::registry& reg, 
	const CatClock& clk,
	const InputData::Mouse& mouse)
{
	//if mouse is not down, can't possibly be clicking on anything
	if (!mouse.l.down && !mouse.r.down && (mouse.scroll == 0)) return;
	//if mouse is not hovering over anything, 
	//can't possibly be clicking on anything
	if (!sound_on_hover_hit.has_value()) return;
	
	entt::entity e = *sound_on_hover_hit;
	
	if (reg.has<DroneSound, GrowableSprite>(e)) {
		DroneSound& drone_sound = reg.get<DroneSound>(e);
		GrowableSprite& growable_sprite = reg.get<GrowableSprite>(e);
		
		if (mouse.l.down) {
			drone_sound.sound.vol += clk.dt_s
				*(1.0f-drone_sound.sound.vol)
				*GROWTH_RATE;
			growable_sprite.y_growth += clk.dt_s
				*(GrowableSprite::MAX_GROWTH-growable_sprite.y_growth)
				*GROWTH_RATE;
		} 
		if (mouse.r.down) {
			drone_sound.sound.vol -= clk.dt_s
				*drone_sound.sound.vol
				*GROWTH_RATE;
			growable_sprite.y_growth -= clk.dt_s
				*growable_sprite.y_growth
				*GROWTH_RATE;
		}
		if (mouse.scroll != 0) {
			if (mouse.scroll > 0) {
				for (int i = 0; i < mouse.scroll; ++i) {
					drone_sound.sound.vol += clk.dt_s
						*(1.0f-drone_sound.sound.vol)
						*GROWTH_RATE;
					growable_sprite.y_growth += clk.dt_s
						*(GrowableSprite::MAX_GROWTH-growable_sprite.y_growth)
						*GROWTH_RATE;
				}
			} else {
				for (int i = 0; i < -mouse.scroll; ++i) {
					drone_sound.sound.vol -= clk.dt_s
						*drone_sound.sound.vol
						*GROWTH_RATE;
					growable_sprite.y_growth -= clk.dt_s
						*growable_sprite.y_growth
						*GROWTH_RATE;
				}
			}
		}
		
	}
}
