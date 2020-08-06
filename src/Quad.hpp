/*
 * Quad.hpp
 *
 *  Created on: Jul 23, 2020
 *      Author: rei de vries
 */

#ifndef SRC_QUAD_HPP_
#define SRC_QUAD_HPP_

#include "CatConf.hpp"
#include "IncludeJson.hpp"
#include "raymath.h"
#include "VectorMath.hpp"

//stores vertices of a quadrilateral for drawing,
//positioned relative to the center of the quad
//some methods assume that the points all lie on the same plane as that is
//my definition of a quad. If they don't, I'd consider it a pair of triangles
//rather than a quad. But it may be possible to use them this way in some cases
//Should also be usable to represent a tri, by setting p[4]=p[3] (untested)
struct Quad {
	// p[0] is top left, rest are clockwise from it
	Vector3 p[4];

	Quad(Rectangle rect, VectorMath::Orthog dir);
	Quad(Rectangle rect) : Quad(rect, VectorMath::Orthog::up) {}
	Quad(Rectangle rect, Vector3 dir);
	Quad() : Quad((Rectangle){0,0,32,32}, VectorMath::Orthog::up) {}
	Quad(const Vector3 set_p[4]);
	
	void scale(float scale);
	void translate(Vector3 offset);
	void rotate(Quaternion q);
	void rotate(float roll, float pitch, float yaw);
	void rotate(Vector3 from, Vector3 to);
	Vector3 getPos3D(Vector2 pos) const; //pos is {0,0} to {1,1}
	Vector3 getCenter() const; // average of a, b, c, d
	RayHitInfo getRayHit(const Ray& r) const;
	RayHitInfo getRayHit(const Ray& r, const Vector3& q_pos) const;
	BoundingBox getBoundingBox() const;
	BoundingBox getBoundingBox(Vector3 origin) const;
	bool checkCollisionRay(const Ray& ray) const;
	std::string print() const;
	
	// assumes points in p all rest on the same plane
	constexpr Vector3 getNormal() const
	{
		return VectorMath::cross(p[2]-p[0], p[1]-p[0]);
	}
};

void to_json(json& j, const Quad& q);
void from_json(const json& j, Quad& q);

#endif /* SRC_QUAD_H_ */
