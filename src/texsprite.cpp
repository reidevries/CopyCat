#include "texsprite.h"

#include <iostream>

using namespace std;

TexSprite::TexSprite(string name, vector<TexRegion> regions, SpriteType type,
	Vector2 size)
{
	this->name = name;
	this->regions = regions;
	this->type = type;
	this->size = size;
	this->offset = (Vector2 ) { 0, 0 };
	this->origin = (Vector2 ) { 0.5, 0.5 };
	draw_index = 0;

	updateRectangleSize();
	updateRectanglePos((Vector2 ) { 0, 0 });

	if (this->type == SpriteType::world) {
		plane = LoadModelFromMesh((Mesh) GenMeshPlane(size.x, size.y, 1, 1));
		setDrawIndex(0);
	}
}

TexSprite::TexSprite(string name, vector<TexRegion> regions, SpriteType type)
	: TexSprite(name, regions, type, regions[0].getDimensions())
{
}

TexSprite::TexSprite(string name, vector<TexRegion> regions)
	: TexSprite(name, regions, SpriteType::screen)
{
}

void TexSprite::setDrawIndex(int tex_index)
{
	if (tex_index < regions.size() && tex_index >= 0) {
		this->draw_index = tex_index;
	} else {
		DebugPrinter::printDebug("TexSprite::setDrawIndex",
			to_string(tex_index) + " is an invalid tex_index for TexSprite "
				+ name, 1);
		cerr << "Tried to set TexSprite '" << name << "' to invalid texindex "
			<< tex_index << endl;
	}

	if (type == SpriteType::world) {
		plane.materials[0].maps[MAP_DIFFUSE].texture =
			regions[draw_index].get();
	}
}

void TexSprite::updateRectanglePos(Vector2 pos)
{
	//changes the position of the drawing rectangle.
	//This won't be correct if updatePixels is not called on updates to scale or offset
	dest_rect.x = pos.x + offset.x;
	dest_rect.y = pos.y + offset.y;
}

void TexSprite::updateRectangleSize()
{
	if (type == SpriteType::world) {
		dest_rect.width = size.x * scale;
		dest_rect.height = size.y * scale;
	} else {
		Rectangle src_rect = regions[draw_index].getSrcRect();
		dest_rect.width = src_rect.width * scale;
		dest_rect.height = src_rect.height * scale;
	}
	origin_pixels = (Vector2 ) {
		dest_rect.width * origin.x,
		dest_rect.height * origin.y
	};
}

void TexSprite::setSize(Vector2 size)
{
	this->size = size;
}

void TexSprite::setOffset(Vector2 offset)
{
	this->offset = offset;
	updateRectangleSize();
	updateRectanglePos((Vector2 ) { });
}

void TexSprite::setScale(float scale)
{
	this->scale = scale;
	updateRectangleSize();
}

void TexSprite::setOrigin(Vector2 origin)
{
	this->scale = scale;
	updateRectangleSize();
}

Texture2D TexSprite::getCurrentTexture()
{
	return regions[draw_index].get();
}

Vector3 TexSprite::getPos3D()
{
	Vector3 pos_3d;
	pos_3d.x = dest_rect.x;
	pos_3d.y = up;
	pos_3d.z = dest_rect.y;
	return pos_3d;
}

void TexSprite::draw(Vector2 pos, Camera cam)
{
	updateRectanglePos(pos);
	regions[draw_index].seen();

	if (type == SpriteType::world) {
		DrawModelEx(plane, getPos3D(), (Vector3 ) { 0.0, 1.0, 0.0 }, rotation,
			(Vector3 ) { 1.0, 1.0, 1.0 }, WHITE);
	} else if (type == SpriteType::billboard) {
		DrawBillboardRec(cam, regions[draw_index].get(),
			regions[draw_index].getSrcRect(), getPos3D(), scale, WHITE);
	} else {
		DebugPrinter::printDebug("TexSprite::draw",
			"pls use drawUI to draw a sprite of the 'screen' type", 3);
	}
}

void TexSprite::drawUI(Color color)
{
	regions[draw_index].seen();
	DrawTexturePro(regions[draw_index].get(), regions[draw_index].getSrcRect(),
		dest_rect, origin_pixels, rotation, color);
}
