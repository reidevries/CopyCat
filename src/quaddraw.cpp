/*
 * catdraw.cpp
 *
 *  Created on: Jul 10, 2020
 *      Author: rei de vries
 */

#include "quaddraw.h"

Cat::Quad::Quad(Rectangle rect)
{
	float h_half = rect.height/2;
	float w_half = rect.width/2;
	a = {rect.x-w_half, rect.y+h_half, rect.x+w_half};
	b = {rect.x+w_half, rect.y+h_half, rect.x-w_half};
	c = {rect.x+w_half, rect.y-h_half, rect.x-w_half};
	d = {rect.x-w_half, rect.y-h_half, rect.x+w_half};
}

Cat::Quad::Quad(Rectangle rect, Orthog dir)
{
	float h_half = rect.height/2;
	float w_half = rect.width/2;

	switch (dir) {
	case up:
	case down:
		a = {rect.x-w_half, 0.0, rect.y-h_half};
		b = {rect.x+w_half, 0.0, rect.y-h_half};
		c = {rect.x+w_half, 0.0, rect.y+h_half};
		d = {rect.x-w_half, 0.0, rect.y+h_half};
		break;
	case north:
	case south:
		a = {0.0, rect.y+h_half, rect.x+w_half};
		b = {0.0, rect.y+h_half, rect.x-w_half};
		c = {0.0, rect.y-h_half, rect.x-w_half};
		d = {0.0, rect.y-h_half, rect.x+w_half};
		break;
	case east:
	case west:
		a = {rect.x-w_half, rect.y+h_half, 0.0};
		b = {rect.x+w_half, rect.y+h_half, 0.0};
		c = {rect.x+w_half, rect.y-h_half, 0.0};
		d = {rect.x-w_half, rect.y-h_half, 0.0};
		break;
	}
}

void Cat::Quad::scale(float scale)
{
	a = VectorMath::scale(a, scale);
	b =	VectorMath::scale(b, scale);
	b =	VectorMath::scale(c, scale);
	d =	VectorMath::scale(d, scale);
}

void Cat::Quad::translate(Vector3 offset)
{
	a = VectorMath::add(a, offset);
	b = VectorMath::add(b, offset);
	c = VectorMath::add(c, offset);
	d = VectorMath::add(d, offset);
}

void Cat::Quad::rotate(Quaternion q)
{
	a = Vector3RotateByQuaternion(a,q);
	b = Vector3RotateByQuaternion(b,q);
	c = Vector3RotateByQuaternion(c,q);
	d = Vector3RotateByQuaternion(d,q);
}

void Cat::Quad::rotate(float roll,float pitch,float yaw)
{
	rotate(QuaternionFromEuler(roll,pitch,yaw));
}

void Cat::Quad::rotate(Vector3 from, Vector3 to)
{
	rotate(QuaternionFromVector3ToVector3(from,to));
}

void Cat::drawQuad(Texture2D tex, Rectangle src_rect,
	Vector3 center, Quad p, Color tint)
{
	if (rlCheckBufferLimit(4)) rlglDraw();

	rlEnableTexture(tex.id);

	rlBegin(RL_QUADS);
		rlColor4ub(tint.r, tint.g, tint.b, tint.a);

		// Bottom-left corner for texture and quad
		rlTexCoord2f(src_rect.x/tex.width,
			src_rect.y/tex.height);
		rlVertex3f(center.x+p.a.x,
			center.y+p.a.y,
			center.z+p.a.z);

		// Top-left corner for texture and quad
		rlTexCoord2f(src_rect.x/tex.width,
			(src_rect.y+src_rect.height)/tex.height);
		rlVertex3f(center.x+p.d.x,
			center.y+p.d.y,
			center.z+p.d.z);

		// Top-right corner for texture and quad
		rlTexCoord2f((src_rect.x+src_rect.width)/tex.width,
			(src_rect.y+src_rect.height)/tex.height);
		rlVertex3f(center.x+p.c.x,
			center.y+p.c.y,
			center.z+p.c.z);

		// Bottom-right corner for texture and quad
		rlTexCoord2f((src_rect.x+src_rect.width)/tex.width,
			src_rect.y/tex.height);
		rlVertex3f(center.x+p.b.x,
			center.y+p.b.y,
			center.z+p.b.z);
	rlEnd();

	rlDisableTexture();
}

//calls above method, with no tint
void Cat::drawQuad(Texture2D tex, Rectangle src_rect,
	Vector3 center, Quad plane)
{
	Color tint = WHITE;
	Cat::drawQuad(tex,src_rect,center,plane, tint);
}

//calls above method, with default (not camera-oriented) values
//, and using the full size of the texture
void Cat::drawQuad(Texture2D tex, Vector3 center, Quad plane, Color tint)
{
	Rectangle src_rect = {
		0,
		0,
		static_cast<float>(tex.width),
		static_cast<float>(tex.height)
	};
	Cat::drawQuad(tex,src_rect,center,plane,tint);
}

//minimum possible version of plane draw call
void Cat::drawQuad(Texture2D tex, Vector3 center, Quad plane)
{
	Color tint = WHITE;
	Cat::drawQuad(tex,center,plane,tint);
}
