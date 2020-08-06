#include "Level.hpp"

#include "VectorMath.hpp"

using namespace std;

void Level::loadTest(entt::registry& reg, ManTex& man_tex, ManAudio& man_audio)
{
	for (int u = -5; u < 5; ++u) {
		for (int v = -4; v < 5; ++v) {
			SpriteAnim sa = man_tex.constructSprite(
                "box_placeholder_generic",
				(Vector2){World::METRE,World::METRE});
			entt::entity e = Factory::makeFloor(
                reg, 
                sa, 
                (Vector3){
                    static_cast<float>(u*World::METRE),
                    0,
                    static_cast<float>(v*World::METRE)
                });
			SoundRes sound_res = man_audio.constructSound("test");
			reg.emplace<HoverSound>(e, sound_res);
		}
	}
}

void Level::load(entt::registry& reg, ManTex& man_tex, std::string filename)
{

}
