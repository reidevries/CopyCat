/*
 * Quad.cpp
 *
 *  Created on: Jul 23, 2020
 *      Author: rei de vries
 */

#include "Quad.hpp"

using namespace std;

Quad::Quad(Rectangle rect, VectorMath::Orthog dir)
{
	float h_half = rect.height/2;
	float w_half = rect.width/2;

	switch (dir) {
	case VectorMath::Orthog::up:
	case VectorMath::Orthog::down:
		a = {rect.x-w_half, 0.0, rect.y-h_half};
		b = {rect.x+w_half, 0.0, rect.y-h_half};
		c = {rect.x+w_half, 0.0, rect.y+h_half};
		d = {rect.x-w_half, 0.0, rect.y+h_half};
		break;
	case VectorMath::Orthog::north:
	case VectorMath::Orthog::south:
		a = {0.0, rect.y+h_half, rect.x+w_half};
		b = {0.0, rect.y+h_half, rect.x-w_half};
		c = {0.0, rect.y-h_half, rect.x-w_half};
		d = {0.0, rect.y-h_half, rect.x+w_half};
		break;
	case VectorMath::Orthog::east:
	case VectorMath::Orthog::west:
		a = {rect.x-w_half, rect.y+h_half, 0.0};
		b = {rect.x+w_half, rect.y+h_half, 0.0};
		c = {rect.x+w_half, rect.y-h_half, 0.0};
		d = {rect.x-w_half, rect.y-h_half, 0.0};
		break;
	case VectorMath::Orthog::facing_cam:
		a = {rect.x-w_half, rect.y+h_half, rect.x-w_half};
		b = {rect.x+w_half, rect.y+h_half, rect.x+w_half};
		c = {rect.x+w_half, rect.y-h_half, rect.x+w_half};
		d = {rect.x-w_half, rect.y-h_half, rect.x-w_half};
		break;
	}
}

Quad::Quad(Rectangle rect, Vector3 dir)
{
	Quad(rect, VectorMath::Orthog::up);
	rotate(orthogToVector3(VectorMath::Orthog::up), dir);
}

void Quad::scale(float scale)
{
	a = VectorMath::scale(a, scale);
	b =	VectorMath::scale(b, scale);
	b =	VectorMath::scale(c, scale);
	d =	VectorMath::scale(d, scale);
}

void Quad::translate(Vector3 offset)
{
	a = VectorMath::add(a, offset);
	b = VectorMath::add(b, offset);
	c = VectorMath::add(c, offset);
	d = VectorMath::add(d, offset);
}

void Quad::rotate(Quaternion q)
{
	a = Vector3RotateByQuaternion(a,q);
	b = Vector3RotateByQuaternion(b,q);
	c = Vector3RotateByQuaternion(c,q);
	d = Vector3RotateByQuaternion(d,q);
}

void Quad::rotate(float roll,float pitch,float yaw)
{
	rotate(QuaternionFromEuler(roll,pitch,yaw));
}

void Quad::rotate(Vector3 from, Vector3 to)
{
	rotate(QuaternionFromVector3ToVector3(from,to));
}

Vector3 Quad::getPos3D(Vector2 pos) const
{
	return VectorMath::fade(
		VectorMath::fade(d,c,pos.x),
		VectorMath::fade(a,b,pos.x),
		pos.y
	);
}

string Quad::print() const
{
	return  "a=" + VectorMath::printVector(a) + "\n"
		+ "\tb=" + VectorMath::printVector(b) + "\n"
		+ "\tc=" + VectorMath::printVector(c) + "\n"
		+ "\td=" + VectorMath::printVector(d) + "\n";
}

void to_json(json& j, const Quad& q)
{
	j = {
		{"Quad.a", q.a},
		{"Quad.b", q.b},
		{"Quad.c", q.c},
		{"Quad.d", q.d}
	};
}

void from_json(const json& j, Quad& q)
{
	j.at("Quad.a").get_to(q.a);
	j.at("Quad.b").get_to(q.b);
	j.at("Quad.c").get_to(q.c);
	j.at("Quad.d").get_to(q.d);
}
