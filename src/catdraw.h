/*
 * catdraw.h
 *
 *  Created on: Jul 10, 2020
 *      Author: rei de vries
 *
 *      Custom rlgl draw calls for stuff raylib didn't come with
 */

#ifndef SRC_CATDRAW_H_
#define SRC_CATDRAW_H_

#include "catconf.h"
#include "rlgl.h"
#include "vectormath.h"

namespace Cat {
	//stores vertices of a plane for drawing,
	//positioned relative to the center of the plane
	//can lock x and y axes to camera
	//in this case, z points to the right
	struct Plane {
		Vector3 a, b, c, d; //a is top left, rest clockwise from a
	};

	//constructs a billboard with size of rectangle
	//and also the offset of rectangle
	constexpr Plane constructPlane(Rectangle rect)
	{

		float h_half = rect.height/2;
		float w_half = rect.width/2;
		return (Plane){
			(Vector3){rect.x-w_half, rect.y+h_half, rect.x+w_half},
			(Vector3){rect.x+w_half, rect.y+h_half, rect.x-w_half},
			(Vector3){rect.x+w_half, rect.y-h_half, rect.x-w_half},
			(Vector3){rect.x-w_half, rect.y-h_half, rect.x+w_half}
		};
	}

	//can use this to get a scaled copy of the plane
	constexpr Plane scalePlane(Plane p, float scale)
	{
		return (Plane){
			VectorMath::scale(p.a, scale),
			VectorMath::scale(p.b, scale),
			VectorMath::scale(p.c, scale),
			VectorMath::scale(p.d, scale)
		};
	}

	constexpr Plane offsetPlane(Plane p, Vector3 offset)
	{
		return (Plane){
			VectorMath::add(p.a, offset),
			VectorMath::add(p.b, offset),
			VectorMath::add(p.c, offset),
			VectorMath::add(p.d, offset)
		};
	}

	Cat::Plane rotatePlane(Plane p, Quaternion q);

	Cat::Plane rotatePlane(Plane p, float roll, float pitch, float yaw);

	void drawPlane(Texture2D tex, Rectangle src_rect,
		Vector3 center, Plane plane, Color tint);
	void drawPlane(Texture2D tex, Rectangle src_rect,
		Vector3 center, Plane plane);
	void drawPlane(Texture2D tex, Vector3 center, Plane plane, Color tint);
	void drawPlane(Texture2D tex, Vector3 center, Plane plane);

};

#endif /* SRC_CUSTOMRLGL_H_ */
