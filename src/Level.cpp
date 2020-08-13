#include "Level.hpp"

using namespace std;


void Level::loadTest(entt::registry& reg, ManTex& man_tex, ManAudio& man_audio)
{
	stringstream ss;
	
	array<array<Fraction,SIZE_Y>, SIZE_X> lattice 
		= genLattice<SIZE_X,SIZE_Y>(COLS,ROWS);

	DebugPrinter::printDebug(1, "Level::loadTest", "loading floor sprite");
	ResSprite floor_sprite = man_tex.constructSprite("mappo",
		"floor",
		16,
		(Vector2){World::METRE, World::METRE});
	
	DebugPrinter::printDebug(1, "Level::loadTest", "loading water sprite");
	ResSprite water_sprite = man_tex.constructSprite("placeholder_water",
		"water",
		16,
		(Vector2){World::METRE, World::METRE});
	
	DebugPrinter::printDebug(1, "Level::loadTest", "loading hill sprite");
	ResSprite hill_sprite = man_tex.constructSprite(
		"hill_placeholder",
		(Vector2){World::METRE,World::METRE});
		
	DebugPrinter::printDebug(1, "Level::loadTest", "loading hover sfx");
	ResSound ping_sound = man_audio.constructSound("ping");
	
	DebugPrinter::printDebug(1, "Level::loadTest", "loading drone sfx");
	ResSound drones[SIZE_X][SIZE_Y];
	for (int u = 0; u < SIZE_X; ++u) {
		for (int v = 0; v < SIZE_Y; ++v) {
			ss.str("");
			ss << "drone" << lattice[u][v].underscoreString();
			drones[u][v] = man_audio.constructSound(ss.str());
			drones[u][v].pitch = 1.0f;
			drones[u][v].vol = 0.0f;
		}
	}
	
	ss.str("");
	ss << "using lattice: " << endl;
	
	for (int u = 0; u < SIZE_X+1; ++u) {
		for (int v = 0; v < SIZE_Y+1; ++v) {
			Vector2 cur_pos = {
				static_cast<float>(u*World::METRE),
				static_cast<float>(v*World::METRE)
			};
			cur_pos = cur_pos + LEVEL_ORIGIN;
		
			int u_m = u % SIZE_X;
			int v_m = v % SIZE_Y;
			
			int rand_16 = (u*69 + ((v*7013)%420)*3)%16;
			
			SpriteAnim rand_floor = {floor_sprite};
			rand_floor.sprite.anim_index = rand_16;
		
			entt::entity floor = Factory::makeFloor(reg, 
                rand_floor,
                (Vector3){
                    cur_pos.x,
                    0,
                    cur_pos.y
                });
			Fraction ji = lattice[u_m][v_m];
			ping_sound.pitch = ji.getf();
			reg.emplace<HoverSound>(floor, ping_sound);
			ss << ji << "\t\t";
			
			entt::entity grow_thingy = Factory::makeGrowThingy(reg,
				SpriteAnim{hill_sprite},
				SpriteAnim{water_sprite},
				drones[u_m][v_m],
				floor);
		}
		ss << endl;
	}
	DebugPrinter::printDebug(4, "Level::loadTest", ss.str());
}

void Level::load(entt::registry& reg, ManTex& man_tex, std::string filename)
{

}
