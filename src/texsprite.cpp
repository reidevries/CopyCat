#include "texsprite.h"

#include <iostream>

TexSprite::TexSprite(std::string name, std::vector<TexRegion> regions, float screenscale) {
	this->name = name;
	this->regions = regions;
	this->offset = (Vector2){0,0};
	this->origin = (Vector2){0.5,0.5};
	draw_index = 0;
	
	setScreenScale(screenscale);
}

void TexSprite::setDrawIndex(int tex_index) {
	if (tex_index < regions.size() && tex_index >= 0) this->draw_index = tex_index;
	else {
		DebugPrinter::printDebug("TexSprite::setDrawIndex",
				std::to_string(tex_index) + " is an invalid tex_index for TexSprite " + name, 1);
		std::cerr << "Tried to set TexSprite '" << name << "' to invalid texindex " << tex_index << std::endl;
	}
}

void TexSprite::updateRectanglePos(Vector2 pos) {
	//changes the position of the drawing rectangle.
	//This won't be correct if updatePixels is not called on updates to scale, screen_scale or offset
	dest_rect.x = scale_pixels*pos.x + offset_pixels.x;
	dest_rect.y = scale_pixels*pos.y + offset_pixels.y;
}

void TexSprite::updatePixels() {
	//the pixel scale is the screen scale multiplied by the scale.
	scale_pixels = scale*screen_scale;
	//the pixel offset is the world offset multiplied by the screen scale
	offset_pixels = VectorMath::scale(offset, screen_scale);

	//the destination rectangle for drawing has the dimensions of the src rectangle multiplied by the pixel scale
	Rectangle src_rect = regions[draw_index].getSrcRect();
	dest_rect.width = src_rect.width*scale_pixels;
	dest_rect.height = src_rect.height*scale_pixels;

	//the dimensions of the destination rectangle are multiplied by the origin position (0 <= x,y <= 1) to get the pixel origin
	origin_pixels = (Vector2){dest_rect.width*origin.x, dest_rect.height*origin.y};
}

void TexSprite::setOffset(Vector2 offset) {
	this->offset = offset;
	updatePixels();
	updateRectanglePos((Vector2){});
}

void TexSprite::setScale(float scale) {
	this->scale = scale;
	updatePixels();
}

void TexSprite::setOrigin(Vector2 origin) {
	this->scale = scale;
	updatePixels();
}

void TexSprite::setScreenScale(float screen_scale) {
	this->screen_scale = screen_scale;
	updatePixels();
}

Texture2D TexSprite::getCurrentTexture() {
	return regions[draw_index].get();
}

void TexSprite::draw(Color color) {
	regions[draw_index].seen();	//this should work with camera position whenever thats implemented
	DrawTexturePro(regions[draw_index].get(), regions[draw_index].getSrcRect(), dest_rect, origin_pixels, rotation, WHITE);
}

void TexSprite::draw(Vector2 pos) {
	updateRectanglePos(pos);
	regions[draw_index].seen(); //same here
	DrawTexturePro(regions[draw_index].get(), regions[draw_index].getSrcRect(), dest_rect, origin_pixels, rotation, WHITE);
}

void TexSprite::draw() {
	draw(WHITE);
}
