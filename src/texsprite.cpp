#include "texsprite.h"

#include <iostream>

using namespace std;

TexSprite::TexSprite(const uint8_t resource_index,
	const string name,
	const Type type,
	vector<uint8_t> regions,
	const Vector2 size)
{
	this->res_id = resource_index;
	this->region_name = name;
	this->type = type;
	this->regions = regions;
	this->size = size;

	setDrawIndex(0);
}

TexSprite::TexSprite(const uint8_t resource_index,
	const string name,
	vector<uint8_t> regions,
	const Vector2 size)
	: TexSprite(resource_index, name, Type::billboard, regions, size)
{
}

TexSprite::TexSprite(const uint8_t resource_index,
	const string name,
	const Vector2 size)
{
	this->res_id = resource_index;
	this->region_name = name;
	this->type = Type::world;
	this->size = size;

	setDrawIndex(0);
}

void TexSprite::setDrawIndex(int tex_index)
{
	if (tex_index < regions.size()
		&& tex_index >= 0
		&& (type == Type::billboard || type == Type::screen)) {
		this->draw_index = tex_index;
	} else {
		DebugPrinter::printDebug(1, "TexSprite::setDrawIndex",
			to_string(tex_index) + " is an invalid tex_index for TexSprite "
			+ region_name);
	}
}

void TexSprite::setRotation(Vector3 rotation)
{
	rotation_axis = VectorMath::normalize_shit_fast(rotation);
	rotation_deg = VectorMath::max(rotation);
}

Vector3 TexSprite::getRotation()
{
	return VectorMath::scale(rotation_axis, rotation_deg);
}

void TexSprite::drawBillboard(Texture2D atlas,
	Rectangle src_rect,
	Vector2 pos,
	Camera cam)
{
	if (type == Type::billboard) {
		DrawBillboardRec(cam, atlas,
			src_rect, getPos3D(pos), 100.0f, WHITE);
	} else {
		DebugPrinter::printDebug(3, "TexSprite::drawBillboard",
			"wrong drawing method used for this SpriteType");
	}
}

void TexSprite::drawWorld(Model model,
	Vector2 pos,
	Camera cam)
{
	if (type == Type::world) {
		DrawModelEx(model, getPos3D(pos), rotation_axis, rotation_deg,
			{scale,scale,scale}, WHITE);
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
			rotation_deg, color);
	} else {
		DebugPrinter::printDebug(3, "TexSprite::drawScreen",
			"wrong drawing method used for this SpriteType");
	}
}
