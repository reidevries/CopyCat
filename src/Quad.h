/*
 * Quad.h
 *
 *  Created on: Jul 23, 2020
 *      Author: rei de vries
 */

#ifndef SRC_QUAD_H_
#define SRC_QUAD_H_

#include "CatConf.h"
#include "raymath.h"
#include "VectorMath.h"

//stores vertices of a quadrilateral for drawing,
//positioned relative to the center of the plane
//can lock x and y axes to camera
//in this case, z points to the right
struct Quad {
	Vector3 a, b, c, d; //a is top left, rest clockwise from a

	Quad(Rectangle rect, VectorMath::Orthog dir);
	Quad(Rectangle rect) : Quad(rect, VectorMath::Orthog::up) {}
	Quad(Rectangle rect, Vector3 dir);
	Quad() : Quad((Rectangle){0,0,32,32}, VectorMath::Orthog::up) {}

	void scale(float scale);
	void translate(Vector3 offset);
	void rotate(Quaternion q);
	void rotate(float roll, float pitch, float yaw);
	void rotate(Vector3 from, Vector3 to);
	Vector3 getPos3D(Vector2 pos) const; //pos is {0,0} to {1,1}
};

#endif /* SRC_QUAD_H_ */
