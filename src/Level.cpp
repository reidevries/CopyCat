#include "Level.hpp"

using namespace std;


void Level::loadTest(entt::registry& reg, ManTex& man_tex, ManAudio& man_audio)
{
	auto lattice = genLattice<SIZE_X,SIZE_Y>(cols, rows);

	SpriteAnim sa = man_tex.constructSprite(
		"box_placeholder_generic",
		(Vector2){World::METRE,World::METRE});
	SoundRes sound_res = man_audio.constructSound("ping");
	stringstream ss;
	ss << "using lattice: " << endl;
	
	for (int u = 0; u < SIZE_X+1; ++u) {
		for (int v = 0; v < SIZE_Y+1; ++v) {
			entt::entity e = Factory::makeFloor(
                reg, 
                sa, 
                (Vector3){
                    static_cast<float>(u*World::METRE),
                    0,
                    static_cast<float>(v*World::METRE)
                });
			sound_res.pitch = lattice[u % SIZE_X][v % SIZE_Y].getf();
			reg.emplace<HoverSound>(e, sound_res);
			ss << static_cast<Fraction>(lattice[u % SIZE_X][v % SIZE_Y]) 
				<< "\t\t";
		}
		ss << endl;
	}
	DebugPrinter::printDebug(4, "Level::loadTest", ss.str());
}

void Level::load(entt::registry& reg, ManTex& man_tex, std::string filename)
{

}
