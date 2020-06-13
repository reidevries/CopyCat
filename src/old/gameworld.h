#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include <raylib.h>
#include <vector>
#include <string>
#include "gameobject.h"
#include "polygon.h"
#include "resman.h"
#include "debugprinter.h"
#include <random>
#include "level.h"
#include "texres.h"

class GameWorld {
	private:
		const int WORLD_H = 128;	//world height is fixed to 128 units. world width is scaled depending on aspect ratio
		int screen_w, screen_h;
		float screen_scale;
		float world_w;
		bool debug;
		Camera cam;
		void pointCameraAt(Vector2 coords);

		std::vector<std::unique_ptr<GameObject>> object_buf; //TODO: replace with "Environment"
		std::vector<std::unique_ptr<TexSprite>> ui_buf;
		Font font;

		std::mt19937 mersenne_twister;
		std::uniform_real_distribution<double> distribute;
	public:
		GameWorld(int screen_w, int screen_h, bool debug);
		void ageTextures(float dt);
		std::unique_ptr<ResMan> update(float dt, unsigned int time_s, std::unique_ptr<ResMan> resman);
		void render(float dt);
		void renderDebug(float dt);
		~GameWorld();
};

#endif
