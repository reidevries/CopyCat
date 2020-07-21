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

#include "CatConf.h"
#include "rlgl.h"

#include "VectorMath.h"

namespace Cat {
	static const Quaternion roll_180 = {0.893997,0,0,-0.448074};
	static const Quaternion pitch_180 = {0,0.893997,0,-0.448074};
	static const Quaternion yaw_180 = {0,0,0.893997,-0.448074};

	/*          up
	 *    west  |  south
	 *        \ | /
	 *          *
	 *         / \
	 *        /   \
	 *   north     east
	 */
	enum Orthog {
		up,
		down,
		north,
		east,
		south,
		west,
		facing_cam //assumes camera is at a specific angle rn
	};
	//finds a Vector3 on the surface of a plane facing the given direction
	constexpr Vector3 posOnPlane(Vector2 v, Orthog dir)
	{
		switch (dir) {
		case up:
			return { v.x, 0.0, v.y };
		case down:
			return {-v.x, 0.0,-v.y };
		case north:
			return { 0.0, v.y, v.x };
		case south:
			return { 0.0,-v.y,-v.y };
		case east:
			return { v.x, v.y, 0.0 };
		case west:
			return {-v.x,-v.y, 0.0 };
		case facing_cam:
			return { v.x, v.y, v.x};
		default:
			return {0,0,0};
		}
	}

	//returns a 3D normal vector facing in the given dir
	constexpr Vector3 orthogToVector3(Orthog dir)
	{
		switch(dir) {
		case up:
			return { 0.0, 1.0, 0.0 };
		case down:
			return { 0.0,-1.0, 0.0 };
		case north:
			return {-1.0, 0.0, 0.0 };
		case south:
			return { 1.0, 0.0, 0.0 };
		case east:
			return { 0.0, 0.0,-1.0 };
		case west:
			return { 0.0, 0.0, 1.0 };
		case facing_cam:
			return {-0.70710678119, 0.0, -0.70710678119};
		default:
			return {0,0,0};
		}
	}

	//stores vertices of a quadrilateral for drawing,
	//positioned relative to the center of the plane
	//can lock x and y axes to camera
	//in this case, z points to the right
	struct Quad {
		Vector3 a, b, c, d; //a is top left, rest clockwise from a

		Quad(Rectangle rect, Orthog dir);
		Quad(Rectangle rect) : Quad(rect, Orthog::up) {}
		Quad(Rectangle rect, Vector3 dir);
		Quad() : Quad((Rectangle){0,0,32,32}, Orthog::up) {}

		void scale(float scale);
		void translate(Vector3 offset);
		void rotate(Quaternion q);
		void rotate(float roll, float pitch, float yaw);
		void rotate(Vector3 from, Vector3 to);
		Vector3 getPos3D(Vector2 pos) const; //pos is {0,0} to {1,1}
	};

	void drawQuad(Texture2D tex, Rectangle src_rect, Vector3 center,
		Quad quad, Color tint);
	void drawQuad(Texture2D tex, Rectangle src_rect, Vector3 center,
		Quad quad);
	void drawQuad(Texture2D tex, Vector3 center, Quad quad, Color tint);
	void drawQuad(Texture2D tex, Vector3 center, Quad quad);

	//draws a tiled quad facing a specific direction, for floors/walls etc
	//havent used or tested it yet
	void drawQuadTiled(Texture2D tex, Rectangle src_rect, Rectangle dest_rect,
		Vector3 center, Orthog dir, Color tint);
};

#endif /* SRC_CUSTOMRLGL_H_ */
