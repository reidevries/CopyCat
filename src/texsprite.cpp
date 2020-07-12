#include "texsprite.h"

#include <iostream>

using namespace std;

TexSprite::TexSprite(const uint8_t res_index,
	const vector<uint8_t> region_indices,
	const string name,
	const Type type,
	const Vector2 size)
{
	this->res_id = res_index;
	this->region_ids = region_indices;
	this->region_name = name;
	this->type = type;
	this->size = size;

	if (type == Type::world) {
		Rectangle square = {0,0,size.x,size.y};
		quad = Cat::Quad(square, Cat::Orthog::north);
	}

	setAnimIndex(0);
}

void TexSprite::setAnimIndex(uint8_t anim_index)
{
	if (anim_index < region_ids.size()) {
		this->anim_index = anim_index;
	}
}

void TexSprite::setRot(Vector3 rot)
{
	rot_axis = VectorMath::normalize_shit_fast(rot);
	rot_deg = VectorMath::max(rot);
}

Vector3 TexSprite::getRot()
{
	return VectorMath::scale(rot_axis, rot_deg);
}

void TexSprite::rotQuad(float roll, float pitch, float yaw)
{
	quad.rotate(roll, pitch, yaw);
}

void TexSprite::drawBillboard(Texture2D atlas,
	Rectangle src_rect,
	Vector2 pos,
	Camera cam)
{
	if (type == Type::billboard) {
		DrawBillboardRec(cam, atlas,
			src_rect, getPos3D(pos), 1.0f, WHITE);
	} else {
		DebugPrinter::printDebug(3, "TexSprite::drawBillboard",
			"wrong drawing method used for this SpriteType");
	}
}

void TexSprite::drawWorld(Texture2D atlas,
	Rectangle src_rect,
	Vector2 pos,
	Camera cam)
{
	if (type == Type::world) {
		Cat::drawQuad(atlas, src_rect, getPos3D(pos), quad);
		//DrawModelEx(model, getPos3D(pos), rotation_axis, rotation_deg,
		//	{scale,scale,scale}, WHITE);
	} else {
		DebugPrinter::printDebug(3, "TexSprite::drawWorld",
			"wrong drawing method used for this SpriteType");
	}
}


void TexSprite::drawScreen(Texture2D atlas,
	Rectangle src_rect,
	const Color color)
{
	if (type == Type::screen) {
		DrawTexturePro(atlas, src_rect, getDestRect({0,0}), getOrigin2D(),
			rot_deg, color);
	} else {
		DebugPrinter::printDebug(3, "TexSprite::drawScreen",
			"wrong drawing method used for this SpriteType");
	}
}
