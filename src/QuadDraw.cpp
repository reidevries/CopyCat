/*
 * QuadDraw.cpp
 *
 *  Created on: Jul 10, 2020
 *      Author: rei de vries
 */

#include "QuadDraw.h"

void ReiDV::drawQuad(Texture2D tex, Rectangle src_rect,
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
void ReiDV::drawQuad(Texture2D tex, Rectangle src_rect,
	Vector3 center, Quad plane)
{
	Color tint = WHITE;
	ReiDV::drawQuad(tex,src_rect,center,plane, tint);
}

//calls above method, with default (not camera-oriented) values
//, and using the full size of the texture
void ReiDV::drawQuad(Texture2D tex, Vector3 center, Quad plane, Color tint)
{
	Rectangle src_rect = {
		0,
		0,
		static_cast<float>(tex.width),
		static_cast<float>(tex.height)
	};
	ReiDV::drawQuad(tex,src_rect,center,plane,tint);
}

//minimum possible version of plane draw call
void ReiDV::drawQuad(Texture2D tex, Vector3 center, Quad plane)
{
	Color tint = WHITE;
	ReiDV::drawQuad(tex,center,plane,tint);
}

//havent used or tested this yet
void ReiDV::drawQuadTiled(Texture2D tex, Rectangle src_rect, Rectangle dest_rect,
	Vector3 center, VectorMath::Orthog dir, Color tint)
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
		Vector3 pos_3D = VectorMath::posOnPlane(pos_uv, dir);

		Vector2 offset_u = {src_rect.width, 0};
		Vector2 offset_v = {src_rect.height,0};
		Vector3 offset_u3D = VectorMath::posOnPlane(offset_u, dir);
		Vector3 offset_v3D = VectorMath::posOnPlane(offset_v, dir);

		pos_3D = VectorMath::sub(pos_3D,
			VectorMath::scale(offset_u3D, x_num_lower));
		pos_3D = VectorMath::sub(pos_3D,
			VectorMath::scale(offset_v3D, y_num_lower));
		pos_3D = VectorMath::add(pos_3D, center);

		for (int u = -x_num_lower; u < x_num_upper; ++u) {
			pos_3D = VectorMath::add(pos_3D, offset_u3D);
			for (int v = -y_num_lower; v < y_num_upper; ++v) {
				pos_3D = VectorMath::add(pos_3D, offset_v3D);
				ReiDV::drawQuad(tex, src_rect, pos_3D, q);
			}
		}
	} else {
		Quad q = Quad(dest_rect, dir);
		ReiDV::drawQuad(tex, src_rect, center, q);
	}
}
