/*
 * Created: 12/08 2020
 *  by Rei de Vries
 */

#include "Systems.hpp"

using namespace std;
using namespace VectorMath;

constexpr float STAMINA_POS_RATE = 0.3f;
constexpr float STAMINA_NEG_RATE = 0.01f;
constexpr float STAMINA_POS_THRESH = 0.95f;
constexpr float STAMINA_NEG_THRESH = 0.3f;
constexpr float WATER_POS_RATE = 1.5f;
constexpr float WATER_NEG_THRESH = 0.4f;
constexpr float WATER_NEG_RATE = 0.02f;
constexpr float WATER_POS_THRESH = 0.8f;
constexpr float FLAP_RATE = 0.4f;//in seconds between flaps

void Systems::seekWater(entt::registry& reg, Bat& bat, const Vector3& bat_pos) 
{
	//first check current target_water to see if it is visible as water
	//if not, remove it as a target, otherwise just return
	if (bat.target_water.has_value()) {
		Water& water = reg.get<Water>(*bat.target_water);
		SpriteAnim& s = reg.get<SpriteAnim>(*bat.target_water);
		if (s.show == true) {
			return;
		} else {
			water.occupied = false;
			bat.target_water.reset();
		}
	}
	
	//search for water by minimum distance, avoiding occupied water
	auto search = reg.view<SpriteAnim, Water, WorldPos>();
	float min_dist = 10000000000.0f;
	std::optional<entt::entity> candidate;
	for (const entt::entity e : search) {
		SpriteAnim& s = search.get<SpriteAnim>(e);
		Water& w = search.get<Water>(e);
		if (s.show == true && !w.occupied) {
			Vector3 e_pos = search.get<WorldPos>(e).pos;
			float e_dist = VectorMath::dist(e_pos, bat_pos);
			if (e_dist < min_dist) {
				min_dist = e_dist;
				candidate = e;
			}
		}
	}
	
	if (candidate.has_value()) {
		stringstream ss;
		ss << "Found water at " << reg.get<WorldPos>(*candidate).pos
			<< " in entity " << static_cast<int>(*candidate) << endl;
		DebugPrinter::printDebug(4, "seekWater (BatAI)", ss.str());
		bat.target_water = candidate;
		reg.get<Water>(*bat.target_water).occupied = true;;
	} 
	if (!bat.target_water.has_value()) {
		bat.target_pos.reset();
	}
}

void Systems::seekHill(entt::registry& reg, Bat& bat, const Vector3& bat_pos)
{
	//first check current target_rest to see if it's a valid hill
	//if not, remove it as target, otherwise just return 
	if (bat.target_rest.has_value()) {
		Hill& hill = reg.get<Hill>(*bat.target_rest);
		SpriteAnim& s = reg.get<SpriteAnim>(*bat.target_rest);
		if (s.show == true) {
			return;
		} else {
			hill.occupied = false;
			bat.target_rest.reset();
		}
	}
	
	//search for hill by minimum distance, avoiding occupied hills
	auto search = reg.view<SpriteAnim, Hill, WorldPos>();
	float min_dist = 10000000000.0f;
	std::optional<entt::entity> candidate;
	for (const entt::entity e : search) {
		SpriteAnim& s = search.get<SpriteAnim>(e);
		Hill& h = search.get<Hill>(e);
		if (s.show == true && !h.occupied) {
			Vector3 e_pos = search.get<WorldPos>(e).pos;
			float e_dist = VectorMath::dist(e_pos, bat_pos);
			if (e_dist < min_dist) {
				min_dist = e_dist;
				candidate = e;
			}
		}
	}

	if (candidate.has_value()) {
		stringstream ss;
		ss << "Found hill at " << reg.get<WorldPos>(*candidate).pos
			<< " in entity " << static_cast<int>(*candidate) << endl;
		DebugPrinter::printDebug(4, "seekHill (BatAI)", ss.str());
		bat.target_rest = candidate;
		reg.get<Hill>(*bat.target_rest).occupied = true;
	} else {
		bat.target_pos.reset();
	}
}

//returns true if dead
bool deplete(Bat& bat, int num_flaps)
{
	for (int i = 0; i < num_flaps; ++i) {
		bat.stamina -= STAMINA_NEG_RATE;
		bat.water -= WATER_NEG_RATE;
	}
	
	return (bat.stamina <= 0 || bat.water <= 0);
}

void flap(Bat& bat,
	ResSound& s,
	ManAudio& man_audio,
	float pitch,
	WorldVel& vel, 
	Vector3 bat_pos,
	Vector3 target_pos,
	int num_flaps)
{
	if (num_flaps > 0) {
		if (bat_pos.y > World::METRE) vel.vel.y = -8.0f;
		else if (bat_pos.y > World::METRE*0.5f) vel.vel.y = -4.0f;
		else vel.vel.y = 0.0f;
		vel.impulse_fall_rate = 0.8f;
		vel.impulse = normalise(target_pos-bat_pos)*256.0f;
		vel.impulse.y += 12.0f;
		if (bat_pos.y < World::METRE && vel.impulse.y < 0) {
			vel.impulse.y = -vel.impulse.y;
		}
		
		man_audio.playSound(s, 0.2f, pitch);
	}
}

void kill(Bat& bat, size_t& num_living_bats)
{
	bat.state = Bat::State::dead;
	if (num_living_bats > 0) --num_living_bats;
}

void Systems::batAI(entt::registry& reg,
		const CatClock& clk,
		const array<array<Fraction,World::SIZE_Y>,World::SIZE_X>& ji,
		ManAudio& man_audio)
{
	auto view = reg.view<Bat, WorldPos, WorldVel, SpriteQuad, SpriteAnim>();
	for (const entt::entity e : view) {
		Bat& bat = view.get<Bat>(e);
		WorldVel& v = view.get<WorldVel>(e);
		Vector3 bat_pos = view.get<WorldPos>(e).pos;
		ResSprite& bat_sprite = view.get<SpriteAnim>(e).sprite;
		bat.flap_timer += clk.dt_s;
		int num_flaps = 0;
		if (bat.flap_timer > FLAP_RATE/2.0f) {
			bat_sprite.anim_index = 0;
		}
		while (bat.flap_timer > FLAP_RATE) {
			bat.flap_timer -= FLAP_RATE;
			++num_flaps;
			bat_sprite.anim_index = 1;
		}
		
		ResSound s;
		auto find_sound = reg.view<AllBatSounds>();
		AllBatSounds all_s= find_sound.get<AllBatSounds>(find_sound.front());
		size_t u_i = static_cast<int>(bat_pos.x/World::METRE);
		size_t v_i = static_cast<int>(bat_pos.y/World::METRE);
		if (u_i >= World::SIZE_X || v_i >= World::SIZE_Y) {
			u_i = 0;
			v_i = 0;
		}
		s = all_s.flute[u_i][v_i];
		float s_p = calcOctaveReducePitch(ji[u_i][v_i]);

		Vector3 normal_to_target;
		float rand_1 = dice.rollF();
		float rand_2 = dice.rollF();
		float rand_3 = dice.rollF();
		Vector3 rand_nearby_pos = Vector3{1-2*rand_1, 0, 1-2*rand_3};
		rand_nearby_pos = rand_nearby_pos*World::METRE*3;
		//stop the target pos being set outside the screen
		if ((rand_nearby_pos.x + bat_pos.x > World::SIZE_X*World::METRE
				&& rand_nearby_pos.x > 0)
			|| (rand_nearby_pos.x + bat_pos.x < 0
				&& rand_nearby_pos.x < 0)) {
			rand_nearby_pos.x = -rand_nearby_pos.x;
		} else if ((rand_nearby_pos.z + bat_pos.z > World::SIZE_Y*World::METRE
				&& rand_nearby_pos.z > 0)
			|| (rand_nearby_pos.z + bat_pos.z < 0
				&& rand_nearby_pos.z < 0)) {
			rand_nearby_pos.z = -rand_nearby_pos.z;
		}
		if (bat_pos.y < World::METRE) {
			rand_nearby_pos.y = rand_2*World::METRE;
		} else if (bat_pos.y > World::METRE*3) {
			rand_nearby_pos.y = -rand_2*World::METRE;
		}
		rand_nearby_pos = rand_nearby_pos + bat_pos;
		Quad hill_quad;
		Vector3 hill_peak;
		
		switch (bat.state) {
		case Bat::State::seeking_water:
			if (deplete(bat, num_flaps)) {
				DebugPrinter::printDebug(4, "Systems::batAI",
					"bat dies while seeking water");
				kill(bat, Systems::num_living_bats);
				break;
			}
			
			if (!bat.target_pos.has_value()) {
				seekWater(reg, bat, bat_pos);
				if (bat.target_water.has_value()) {
					bat.target_pos = reg.get<WorldPos>(*bat.target_water).pos;
					bat.target_pos = *bat.target_pos 
						+ Vector3{1,World::METRE*0.4f,1};
				}
			}
			
			if (bat.target_pos.has_value()) {
				if (dist(bat_pos, *bat.target_pos) < 1+rand_2*2) {
					DebugPrinter::printDebug(4, "Systems::batAI","bat drinks");
					bat.state = Bat::State::drinking;
					v.impulse = {0,0,0};
					v.vel = {0,0,0};
					bat.target_pos.reset();
					break;
				} else {
					flap(bat, s, man_audio, s_p, v, bat_pos, *bat.target_pos, num_flaps);
				}
			}
			break;
		case Bat::State::seeking_rest:
			if (deplete(bat, num_flaps)) {
				DebugPrinter::printDebug(4, "Systems::batAI",
					"bat dies while seeking somewhere to rest");
				kill(bat, Systems::num_living_bats);
				break;
			}
			if (!bat.target_pos.has_value()) {
				seekHill(reg, bat, bat_pos);
				if (bat.target_rest.has_value()) {
					hill_quad = reg.get<SpriteQuad>(*bat.target_rest).quad;
					hill_peak = hill_quad.p[0]+hill_quad.p[1];
					hill_peak = hill_peak^Vector3{0.5f,0.3f,0.5f};
					bat.target_pos = reg.get<WorldPos>(*bat.target_rest).pos;
					bat.target_pos = *bat.target_pos + hill_peak;
					bat.target_pos = *bat.target_pos 
						+ Vector3{1,World::METRE*0.4f,1};
				}
			}
			
			if (bat.target_pos.has_value()) {
				if (dist(bat_pos, *bat.target_pos) < 1+rand_1*2) {
					DebugPrinter::printDebug(4, "Systems::batAI","bat sleeps");
					bat.state = Bat::State::sleeping;
					v.impulse = {0,0,0};
					v.vel = {0,0,0};
					bat.target_pos.reset();
					break;
				} else {
					flap(bat, s, man_audio, s_p, v, bat_pos, *bat.target_pos, num_flaps);
				}
			}
			break;
		case Bat::State::flying:
			if (deplete(bat, num_flaps)) {
				DebugPrinter::printDebug(4, "Systems::batAI",
					"bat dies while flying around aimlessly");
				kill(bat, Systems::num_living_bats);
				break;
			}
			
			if (bat.target_pos.has_value()) {
				flap(bat, s, man_audio, s_p, v, bat_pos, *bat.target_pos, num_flaps);
				if (dist(bat_pos, *bat.target_pos) < 1) {
					bat.target_pos = rand_nearby_pos;
				}
			} else {
				bat.target_pos = rand_nearby_pos;
			}
			if (bat.water < WATER_NEG_THRESH-rand_1) {
				DebugPrinter::printDebug(4,"Systems::batAI","bat seeks water");
				bat.state = Bat::State::seeking_water;
				bat.target_pos.reset();
			} else if (bat.stamina < STAMINA_NEG_THRESH-rand_2) {
				DebugPrinter::printDebug(4,"Systems::batAI","bat seeks rest");
				bat.state = Bat::State::seeking_rest;
				bat.target_pos.reset();
			}
			break;
		case Bat::State::drinking:
			for (int i = 0; i < num_flaps; ++i) {
				bat.water += WATER_POS_RATE*clk.dt_s;
			}
			bat_sprite.anim_index = 2;
			if (bat.water > WATER_POS_THRESH+rand_1) {
				DebugPrinter::printDebug(4,"Systems::batAI","bat idles");
				bat.state = Bat::State::flying;
				if (bat.target_water.has_value()) {
					reg.get<Water>(*bat.target_water).occupied = false;
					bat.target_water.reset();
				}
			}
			break;
		case Bat::State::sleeping:
			bat_sprite.anim_index = 1;
			for (int i = 0; i < num_flaps; ++i) {
				bat.stamina += STAMINA_POS_RATE*clk.dt_s;
			}
			if (bat.stamina > STAMINA_POS_THRESH+rand_2) {
				DebugPrinter::printDebug(4,"Systems::batAI","bat idles");
				bat.state = Bat::State::flying;
				if (bat.target_rest.has_value()) {
					reg.get<Hill>(*bat.target_rest).occupied = false;
					bat.target_rest.reset();
				}
			}
			break;
		case Bat::State::dead:
			bat_sprite.anim_index = 3;
			if (bat_pos.y > World::METRE*0.5f) v.vel.y = -64.0f;
			else {
				v.vel.y = 0.0;
				v.impulse.y = 0.0;
				view.get<WorldPos>(e).pos.y = World::METRE*0.5f;
			}
			
			break;
		}
	}
}
