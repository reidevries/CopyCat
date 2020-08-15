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

#include <entt/entity/registry.hpp>

#include "CatConf.hpp"
#include "CatClock.hpp"
#include "IncludeComponents.hpp"
#include "DebugPrinter.hpp"
#include "Environment.hpp"
#include "ResBuf.hpp"
#include "ManTex.hpp"
#include "QuadDraw.hpp"
#include "WorldConstants.hpp"

class ViewRenderer
{
public:
	int screen_w, screen_h;
	int world_w;
	float screen_scale;
	const bool debug;
	Camera cam;
	void pointCameraAt(const Vector2 coords);
	Rectangle getCameraFrustrum();

	Font font;

	void renderAxes();
	void renderAxes2D(Vector2 pos, float cam_rot);
	void renderDebug(CatClock& clk, entt::registry& reg, ManTex& man_tex);
public:
	ViewRenderer(const int screen_w, const int screen_h,
		const bool set_debug);
	void updateScreenWH(const int w, const int h) {screen_w=w; screen_h=h;}
	void render(CatClock& clk, entt::registry& reg, ManTex& man_tex);
	const Camera& getCam() const {return cam;}
};

#endif /* SRC_VIEWRENDERER_H_ */
