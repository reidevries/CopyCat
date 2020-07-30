#include "Level.hpp"

#include "VectorMath.hpp"

using namespace std;

void Level::loadTest(entt::registry& reg, ResMan& res_man)
{
	vector<string> names = res_man.getRegionNames("tile/floor");
	int name_index = 2;
	for (int u = -10; u < 10; u=u+2) {
		for (int v = -10; v < 10; v=v+2) {
			SpriteAnim sa = res_man.constructSprite(string("tile/floor"),
				names[name_index],
				1,
				(Vector2){16,16});
			Factory::makeFloor(reg, sa, (Vector3){u*16.0f,0,v*16.0f});
			if ((name_index % 3) <2) name_index--;
		}
		name_index = (name_index+6) % names.size();
	}
}

void Level::load(entt::registry& reg, ResMan& res_man, std::string filename)
{

}
