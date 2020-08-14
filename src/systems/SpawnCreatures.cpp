/*
 * Created: 13/08 2020
 *  by Rei de Vries
 */

#include "Systems.hpp"

using namespace std;
using namespace VectorMath;

void Systems::spawnCreatures(entt::registry& reg, 
	const CatClock& clk,
	ManTex& man_tex,
	ManAudio& man_audio)
{
	if (!clk.tock_beat) return;
	if (dice.rollB(0.5)) return;
	if (num_living_bats >= num_active_hills) return;
	
	stringstream ss;
	ss << "spawned bat #" << num_living_bats << " with " 
		<< num_active_hills << " hills." << endl;
	DebugPrinter::printDebug(4, "Systems::spawnCreatures", ss.str());
	
	ResSprite bat_sprite = man_tex.constructSprite(
		"creature", 
		"bat",
		4, 
		Vector2{World::METRE*0.25f, World::METRE*0.25f});
	
	//spawn only on the map's edges
	Vector3 spawn_point = {
		World::SIZE_X*World::METRE,
		80.0f,
		World::SIZE_Y*World::METRE
	};
	if (dice.rollB(0.5f)) {
		spawn_point.x *= dice.rollF();
		if (dice.rollB(0.5f)) spawn_point.y = 0;
	} else {
		spawn_point.y *= dice.rollF();
		if (dice.rollB(0.5f)) spawn_point.x = 0;
	}
	
	Factory::makeBat(reg,
		spawn_point,
		SpriteAnim{bat_sprite});
	++num_living_bats;
}
