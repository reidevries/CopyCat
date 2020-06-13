/*
 * viewrenderer.h
 *
 *  Created on: Jun 13, 2020
 *      Author: rei de vries
 */

#ifndef SRC_VIEWRENDERER_H_
#define SRC_VIEWRENDERER_H_

#include "raylib.h"
#include <vector>
#include <set>
#include <string>
#include "gameobject.h"
#include "polygon.h"
#include "resman.h"
#include "debugprinter.h"
#include "environment.h"
#include "level.h"
#include "texres.h"

class ViewRenderer
{
public:
	//world height is fixed to 128 units.
	//world width is scaled depending on aspect ratio
	const int WORLD_H = 128;
	int screen_w, screen_h;
	float screen_scale;
	float world_w;
	bool debug;
	Camera cam;
	void pointCameraAt(Vector2 coords);
	Rectangle getCameraFrustrum();

	std::vector<std::unique_ptr<TexSprite>> ui_buf;
	Font font;

public:
	ViewRenderer(int screen_w, int screen_h, bool debug);
	void render(float dt, Environment& environment);
	void renderDebug(float dt);
};

#endif /* SRC_VIEWRENDERER_H_ */
