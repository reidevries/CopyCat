/*
 * catdraw.cpp
 *
 *  Created on: Jul 10, 2020
 *      Author: rei de vries
 */

#include "QuadDraw.h"

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
	case facing_cam:
		a = {rect.x-w_half, rect.y+h_half, rect.x-w_half};
		b = {rect.x+w_half, rect.y+h_half, rect.x+w_half};
		c = {rect.x+w_half, rect.y-h_half, rect.x+w_half};
		d = {rect.x-w_half, rect.y-h_half, rect.x-w_half};
		break;
	}
}

Cat::Quad::Quad(Rectangle rect, Vector3 dir)
{
	Quad(rect, Orthog::up);
	rotate(orthogToVector3(Orthog::up), dir);
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

Vector3 Cat::Quad::getPos3D(Vector2 pos) const
{
	return VectorMath::fade(
		VectorMath::fade(d,c,pos.x),
		VectorMath::fade(a,b,pos.x),
		pos.y
	);
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

//havent used or tested this yet
void Cat::drawQuadTiled(Texture2D tex, Rectangle src_rect, Rectangle dest_rect,
	Vector3 center, Orthog dir, Color tint)
{

	if (VectorMath::rectContains(src_rect, dest_rect)) {
		Quad q = Quad(src_rect, dir);
		int x_num = dest_rect.width/src_rect.width;
		int x_num_lower = x_num/2;
		int x_num_upper = x_num-x_num/2;
		int y_num = dest_rect.height/src_rect.height;
		int y_num_lower = x_num/2;
		int y_num_upper = x_num-x_num/2;

		Vector2 pos_uv = {
			src_rect.width/2,
			src_rect.height/2
		};
		Vector3 pos_3D = posOnPlane(pos_uv, dir);

		Vector2 offset_u = {src_rect.width, 0};
		Vector2 offset_v = {src_rect.height,0};
		Vector3 offset_u3D = posOnPlane(offset_u, dir);
		Vector3 offset_v3D = posOnPlane(offset_v, dir);

		pos_3D = VectorMath::sub(pos_3D,
			VectorMath::scale(offset_u3D, x_num_lower));
		pos_3D = VectorMath::sub(pos_3D,
			VectorMath::scale(offset_v3D, y_num_lower));
		pos_3D = VectorMath::add(pos_3D, center);

		for (int u = -x_num_lower; u < x_num_upper; ++u) {
			pos_3D = VectorMath::add(pos_3D, offset_u3D);
			for (int v = -y_num_lower; v < y_num_upper; ++v) {
				pos_3D = VectorMath::add(pos_3D, offset_v3D);
				Cat::drawQuad(tex, src_rect, pos_3D, q);
			}
		}
	} else {
		Quad q = Quad(dest_rect, dir);
		Cat::drawQuad(tex, src_rect, center, q);
	}
}
