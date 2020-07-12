/*
 * viewrenderer.h
 *
 *  Created on: Jun 13, 2020
 *      Author: rei de vries
 */

#ifndef SRC_VIEWRENDERER_H_
#define SRC_VIEWRENDERER_H_

#include <vector>
#include <set>
#include <string>

#include "catconf.h"
#include "gameobject.h"
#include "polygon.h"
#include "resman.h"
#include "debugprinter.h"
#include "environment.h"
#include "level.h"
#include "catclock.h"
#include "resbuf.h"

class ViewRenderer
{
public:
	//world height is fixed to 128 units.
	//world width is scaled depending on aspect ratio
	static const int WORLD_H = 128;
	static const int METRE = 16;
	int screen_w, screen_h;
	float screen_scale;
	float world_w;
	const bool debug;
	Camera cam;
	void pointCameraAt(const Vector2 coords);
	Rectangle getCameraFrustrum();

	Model testmodel;

	std::vector<TexSprite> ui_buf;
	Font font;

	void renderAxes();
	void renderAxes2D(Vector2 pos, float cam_rot);
	void renderDebug(CatClock& clk, Environment& environment, ResMan& resman);
public:
	ViewRenderer(const int screen_w, const int screen_h,
		const bool set_debug);
	void addSprite(TexSprite ui_sprite);
	void render(CatClock& clk, Environment& environment, ResMan& resman);
	void drawObject(std::shared_ptr<GameObject> object, ResMan& resman);
};

#endif /* SRC_VIEWRENDERER_H_ */
