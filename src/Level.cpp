#include "Level.h"

#include "VectorMath.h"

using namespace std;

void Level::loadTest(entt::registry& reg, ResMan& res_man)
{
	for (int u = -10; u < 10; u=u+2) {
		for (int v = -10; v < 10; v=v+2) {
			Cat::makeFloor(reg,
				res_man.constructSprite(string("tiles"),
					string("floor"),
					1,
					(Vector2){16,16}),
				(Vector3){u*16,0,v*16},
				(Rectangle){0,0,16,16}
				);
		}
	}
}

void Level::load(entt::registry& reg, ResMan& res_man, std::string filename)
{

}
