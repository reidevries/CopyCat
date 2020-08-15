/*
 * quaddraw.h
 *
 *  Created on: Jul 10, 2020
 *      Author: rei de vries
 *
 *      Custom rlgl draw calls for drawing arbitrary quarilaterals
 */

#ifndef SRC_QUADDRAW_H_
#define SRC_QUADDRAW_H_

#include "CatConf.hpp"
#include "rlgl.h"
#include "Quad.hpp"

#include "VectorMath.hpp"

namespace ReiDV {
	void drawQuad(Texture2D tex, Rectangle src_rect, Vector3 center,
		Quad quad, Color tint, bool flip_x, bool flip_y);
	void drawQuad(Texture2D tex, Rectangle src_rect, Vector3 center,
		Quad quad, Color tint);
	void drawQuad(Texture2D tex, Rectangle src_rect, Vector3 center,
		Quad quad);
	void drawQuad(Texture2D tex, Vector3 center, Quad quad, Color tint);
	void drawQuad(Texture2D tex, Vector3 center, Quad quad);

	//draws a tiled quad facing a specific direction, for floors/walls etc
	//havent used or tested it yet
	void drawQuadTiled(Texture2D tex, Rectangle src_rect, Rectangle dest_rect,
		Vector3 center, VectorMath::Orthog dir, Color tint);
};

#endif /* SRC_CUSTOMRLGL_H_ */
