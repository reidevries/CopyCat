/*
 * catdraw.cpp
 *
 *  Created on: Jul 10, 2020
 *      Author: rei de vries
 */

#include "catdraw.h"

Cat::Plane Cat::rotatePlane(Plane p,Quaternion q)
{
	return (Plane){
		Vector3RotateByQuaternion(p.a,q),
		Vector3RotateByQuaternion(p.b,q),
		Vector3RotateByQuaternion(p.c,q),
		Vector3RotateByQuaternion(p.d,q)
	};
}

Cat::Plane Cat::rotatePlane(Plane p,float roll,float pitch,float yaw)
{
	return rotatePlane(p, QuaternionFromEuler(roll,pitch,yaw));
}

void Cat::drawPlane(Texture2D tex, Rectangle src_rect,
	Vector3 center, Plane p, Color tint)
{
	if (rlCheckBufferLimit(4)) rlglDraw();

	rlEnableTexture(tex.id);

	rlBegin(RL_QUADS);
		rlColor4ub(tint.r, tint.g, tint.b, tint.a);

		// Bottom-left corner for texture and quad
		rlTexCoord2f(src_rect.x/tex.width,
			src_rect.y/tex.height);
		rlVertex3f(p.a.x, p.a.y, p.a.z);

		// Top-left corner for texture and quad
		rlTexCoord2f(src_rect.x/tex.width,
			(src_rect.y+src_rect.height)/tex.height);
		rlVertex3f(p.d.x, p.d.y, p.d.z);

		// Top-right corner for texture and quad
		rlTexCoord2f((src_rect.x+src_rect.width)/tex.width,
			(src_rect.y+src_rect.height)/tex.height);
		rlVertex3f(p.c.x, p.c.y, p.c.z);

		// Bottom-right corner for texture and quad
		rlTexCoord2f((src_rect.x+src_rect.width)/tex.width,
			src_rect.y/tex.height);
		rlVertex3f(p.b.x, p.b.y, p.b.z);
	rlEnd();

	rlDisableTexture();
}

//calls above method, with no tint
void Cat::drawPlane(Texture2D tex, Rectangle src_rect,
	Vector3 center, Plane plane)
{
	Color tint = WHITE;
	Cat::drawPlane(tex,src_rect,center,plane, tint);
}

//calls above method, with default (not camera-oriented) values
//, and using the full size of the texture
void Cat::drawPlane(Texture2D tex, Vector3 center, Plane plane, Color tint)
{
	Rectangle src_rect = {
		0,
		0,
		static_cast<float>(tex.width),
		static_cast<float>(tex.height)
	};
	Cat::drawPlane(tex,src_rect,center,plane,tint);
}

//minimum possible version of plane draw call
void Cat::drawPlane(Texture2D tex, Vector3 center, Plane plane)
{
	Color tint = WHITE;
	Cat::drawPlane(tex,center,plane,tint);
}
