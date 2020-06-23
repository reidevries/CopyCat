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
#include "catclock.h"

class ViewRenderer
{
public:
	//world height is fixed to 128 units.
	//world width is scaled depending on aspect ratio
	const int WORLD_H = 128;
	int screen_w, screen_h;
	float screen_scale;
	float world_w;
	const bool debug;
	Camera cam;
	void pointCameraAt(const Vector2 coords);
	Rectangle getCameraFrustrum();

	Model testmodel;

	std::vector<std::unique_ptr<TexSprite>> ui_buf;
	Font font;

public:
	ViewRenderer(const int screen_w, const int screen_h,
		const bool set_debug);
	void render(CatClock& clk, Environment& environment);
	void renderDebug(CatClock& clk, Environment& environment);
};

#endif /* SRC_VIEWRENDERER_H_ */
