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

#include "catconf.h"
#include "rlgl.h"
#include "vectormath.h"

namespace Cat {
	static const Quaternion roll_180 = {0.893997,0,0,-0.448074};
	static const Quaternion pitch_180 = {0,0.893997,0,-0.448074};
	static const Quaternion yaw_180 = {0,0,0.893997,-0.448074};

	enum Orthog {
		up,
		down,
		north,
		east,
		south,
		west
	};

	//stores vertices of a quadrilateral for drawing,
	//positioned relative to the center of the plane
	//can lock x and y axes to camera
	//in this case, z points to the right
	struct Quad {
		Vector3 a, b, c, d; //a is top left, rest clockwise from a

		Quad(Rectangle rect);
		Quad(Rectangle rect, Orthog dir);
		Quad() : Quad((Rectangle){0,0,32,32}, Orthog::up) {}

		void scale(float scale);
		void translate(Vector3 offset);
		void rotate(Quaternion q);
		void rotate(float roll, float pitch, float yaw);
		void rotate(Vector3 from, Vector3 to);
		//if the quad is invisible due to facing away from screen, this
		//will flip it 180 degrees to face the screen
		//also, if the quad is not formed properly, it will iron out the kinks
	};

	void drawQuad(Texture2D tex, Rectangle src_rect, Vector3 center,
		Quad quad, Color tint);
	void drawQuad(Texture2D tex, Rectangle src_rect, Vector3 center,
		Quad quad);
	void drawQuad(Texture2D tex, Vector3 center, Quad quad, Color tint);
	void drawQuad(Texture2D tex, Vector3 center, Quad quad);
};

#endif /* SRC_CUSTOMRLGL_H_ */
