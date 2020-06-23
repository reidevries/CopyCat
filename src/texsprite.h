/*
 * texturesprite.h
 *
 *      Author: rei de vries
 *	This class represents a sprite to be drawn on the screen.
 *	It can draw from multiple textures, and multiple regions
 *	of the same texture. draw_index can be modified to change
 *	which region is drawn on the screen at any time, which
 *	makes it easy to implement animations and stuff without
 *	having to constantly switch out resources.
 */

#ifndef TEXTURESPRITE_H
#define TEXTURESPRITE_H

#include <raylib.h>
#include <string>
#include <memory>
#include <vector>
#include "debugprinter.h"

#include "texregion.h"
#include "vectormath.h"

//defines how the sprite is to be drawn, only for use within TexSprite
enum class SpriteType {
	screen,		//Gets drawn in 2D by drawUI
				//It is drawn on the screen at the pixel coords of "offset"
				//This is a candidate for being split into another class, but
				//I think it would share too many methods

	world,  	//Gets drawn in 3D as a flat plane textured with the sprite
				//It's drawn in the world at the coordinates specified by
				//dest_rect and "up", which is formed from
				//offset+pos as specified in draw(Vector2, Camera)

	billboard 	//gets drawn in 3D as a billboard
				//It's drawn in the same way as "world", but it always faces
				//the camera. Can't be rotated on any axis due to limitations
				//of the raylib library.
};

class TexSprite {
private:
	std::string name;
	SpriteType type;

	Model plane; //cant be static because it also contains the "material"
	Vector2 size;
	float up; //determines y position if rendered in 3D
	/* This vector stores all the regions of textures that can be drawn
	 * by this texsprite. Using the same shared_ptr to draw multiple
	 * regions should be fine, because it will automatically handle
	 * deletion without having to store multiple copies of the resources.
	 * src_rect is ignored for "world" type drawing
	 */
	std::vector<TexRegion> regions;
	int draw_index; //the index of the texture that currently being drawn
	Vector2 offset;
	//the origin for rotation and positioning
	Vector2 origin; //0x0 = top left corner, 1x1=bottom right corner
	float scale = 1;
	float rotation = 0;

	Vector2 origin_pixels;
	Rectangle dest_rect;
	//update rectangle size
	void updateRectangleSize();
	//updates the position to draw on screen
	void updateRectanglePos(Vector2 pos);

	Texture2D getCurrentTexture();
	void ageTextures();
	Vector3 getPos3D() const;	//get the pos as 3D world coords
public:
	TexSprite(std::string name,
			SpriteType type,
			std::vector<TexRegion> regions,
			Vector2 size);
	//same as above, but using src_rect size as default
	TexSprite(std::string name,
			SpriteType type,
			std::vector<TexRegion> regions);
	//same as above, but defaulting to "screen" SpriteType
	TexSprite(std::string name,
			std::vector<TexRegion> regions);
	std::vector<TexRegion> getAllTextures() {return regions;}
	std::string getName() {return name;}

	//also updates material texture
	void setDrawIndex(int tex_index);
	int getDrawIndex() const {return draw_index;}
	void setOffset(Vector2 offset);
	Vector2 getOffset() const {return offset;}
	void setSize(Vector2 size);
	Vector2 getSize() const {return size;}
	void setOrigin(Vector2 origin);
	Vector2 getOrigin() const {return origin;}
	void setScale(float scale);
	float getScale() const {return scale;}
	void setRotation(float rotation) {this->rotation = rotation;}
	float getRotation() const {return rotation;}

	void draw(Vector2 pos, Camera cam);
	void drawUI(Color color);
};

#endif
