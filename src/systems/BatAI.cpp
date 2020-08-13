/*
 * Created: 12/08 2020
 *  by Rei de Vries
 */

#include "Systems.hpp"

using namespace std;
using namespace VectorMath;

constexpr float STAMINA_RATE = 0.01f;
constexpr float STAMINA_NEG_THRESH = 0.5f;
constexpr float STAMINA_POS_THRESH = 0.95f;
constexpr float WATER_RATE = 0.01f;
constexpr float WATER_NEG_THRESH = 0.3f;
constexpr float WATER_POS_THRESH = 0.8f;
constexpr float FLAP_RATE = 1.2f;//in seconds between flaps

template<typename T> //T = Water or Hill (pls dont use it for anything else)
optional<entt::entity> search(entt::registry& reg, const Vector3& origin_pos)
{
	auto search = reg.view<SpriteAnim, T, WorldPos>();
	float min_dist = 10000000000.0f;
	optional<entt::entity> candidate;
	for (const entt::entity e : search) {
		SpriteAnim& s = search.template get<SpriteAnim>(e);
		if (s.show == true) {
			Vector3 e_pos = search.template get<WorldPos>(e).pos;
			float e_dist = dist(e_pos, origin_pos);
			if (e_dist < min_dist) {
				min_dist = e_dist;
				candidate = e;
			}
		}
	}
	return candidate;
}

void seekWater(entt::registry& reg, Bat& bat, const Vector3& bat_pos) 
{
	//first check current target_water to see if it is visible as water
	//if not, remove it as a target, otherwise just return
	if (bat.target_water.has_value()) {
		SpriteAnim& s = reg.get<SpriteAnim>(*bat.target_water);
		if (s.show == true) return;
		else bat.target_water.reset();
	}
	
	bat.target_water = search<Water>(reg, bat_pos);
}

void seekHill(entt::registry& reg, Bat& bat, const Vector3& bat_pos)
{
	//first check current target_rest to see if it's a valid hill
	//if not, remove it as target, otherwise just return 
	if (bat.target_rest.has_value()) {
		SpriteAnim& s = reg.get<SpriteAnim>(*bat.target_rest);
		if (s.show == true) return;
		else bat.target_rest.reset();
	}
	
	bat.target_rest = search<Hill>(reg, bat_pos);
}

//returns true if dead
bool deplete(Bat& bat, int num_flaps)
{
	for (int i = 0; i < num_flaps; ++i) {
		bat.stamina -= STAMINA_RATE;
		bat.water -= WATER_RATE;
	}
	
	return (bat.stamina <= 0 || bat.water <= 0);
}

void flap(Bat& bat,
	WorldVel& vel, 
	Vector3 bat_pos,
	Vector3 target_pos,
	int num_flaps)
{
	if (num_flaps > 0) {
		vel.vel.y = -0.5f;
		vel.impulse = normalise(target_pos-bat_pos)*2.0f;
		vel.impulse.y = 1.0f;
		bat.just_flapped = true;
	}
}

void Systems::batAI(entt::registry& reg,
		const CatClock& clk,
		const array<array<Fraction,World::SIZE_Y>,World::SIZE_X>& ji,
		ManAudio& man_audio)
{
	auto view = reg.view<Bat, WorldPos, WorldVel, SpriteQuad>();
	for (const entt::entity e : view) {
		Bat& bat = view.get<Bat>(e);
		WorldVel& v = view.get<WorldVel>(e);
		Vector3 bat_pos = view.get<WorldPos>(e).pos;
		bat.flap_timer += clk.dt_s;
		int num_flaps = 0;
		while (bat.flap_timer > FLAP_RATE) {
			bat.flap_timer -= FLAP_RATE;
			++num_flaps;
		}

		Vector3 target_pos;
		Vector3 normal_to_target;
		float rand_1 = dice.rollF();
		float rand_2 = dice.rollF();
		float rand_3 = dice.rollF();
		
		switch (bat.state) {
		case Bat::State::seeking_water:
			if (deplete(bat, num_flaps)) {
				bat.state = Bat::State::dead;
				break;
			}
			seekWater(reg, bat, bat_pos);
			if (dist(bat_pos, target_pos) < 1) {
				bat.state = Bat::State::drinking;
				v.impulse = {0,0,0};
				v.vel = {0,0,0};
				break;
			} else {
				target_pos = reg.get<WorldPos>(*bat.target_water).pos;
				flap(bat, v, bat_pos, target_pos, num_flaps);
			}
			break;
		case Bat::State::seeking_rest:
			if (deplete(bat, num_flaps)) {
				bat.state = Bat::State::dead;
				break;
			}
			seekHill(reg, bat, bat_pos);
			if (dist(bat_pos, target_pos) < 1) {
				bat.state = Bat::State::sleeping;
				v.impulse = {0,0,0};
				v.vel = {0,0,0};
				break;
			} else {
				target_pos = reg.get<WorldPos>(*bat.target_rest).pos;
				flap(bat, v, bat_pos, target_pos, num_flaps);
			}
			break;
		case Bat::State::flying:
			if (deplete(bat, num_flaps)) {
				bat.state = Bat::State::dead;
				break;
			}
			flap(bat, v, bat_pos, bat.target_idle, num_flaps);
			if (bat.water < WATER_NEG_THRESH+rand_1) {
				bat.state = Bat::State::seeking_water;
			} else if (bat.stamina < STAMINA_NEG_THRESH+rand_2) {
				bat.state = Bat::State::seeking_rest;
			} else if (dist(bat_pos, bat.target_idle) < 1) {
				bat.target_idle = {
					rand_1*World::SIZE_X+World::ORIGIN_X,
					rand_2, 
					rand_3*World::SIZE_Y+World::ORIGIN_Y
				};
			}
			break;
		case Bat::State::drinking:
			bat.water += WATER_RATE*clk.dt_s;
			if (bat.water > WATER_POS_THRESH+rand_1) {
				bat.state = Bat::State::flying;
				bat.target_idle = {
					rand_1*World::SIZE_X+World::ORIGIN_X,
					rand_2, 
					rand_3*World::SIZE_Y+World::ORIGIN_Y
				};
			}
			break;
		case Bat::State::sleeping:
			bat.stamina += STAMINA_RATE*clk.dt_s;
			if (bat.stamina > STAMINA_POS_THRESH+rand_2) {
				bat.state = Bat::State::flying;
				bat.target_idle = {
					rand_1*World::SIZE_X+World::ORIGIN_X,
					rand_2, 
					rand_3*World::SIZE_Y+World::ORIGIN_Y
				};
			}
			break;
		case Bat::State::dead:
			if (bat_pos.y > 0.1f) v.vel.y = -1.0f;
			else {
				v.vel.y = 0.0;
				v.impulse.y = 0.0;
				view.get<WorldPos>(e).pos.y = 0.01f;
			}
			
			break;
		}
	}
}
