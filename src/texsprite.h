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

#include <string>
#include <memory>
#include <vector>
#include <cstdint>

#include "catconf.h"
#include "debugprinter.h"
#include "vectormath.h"


class TexSprite {
public:
	//defines how the sprite is to be drawn
	enum class Type {
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

private:
	//ID of the Texture2D or Model from ResMan
	//if type==world it references a Model
	//otherwise it's a Texture2D
	uint8_t res_id;
	std::string region_name;
	Type type;

	//list of regions from the atlas, and the index of which one to draw
	//only used if type is 'billboard' or 'screen'
	std::vector<uint8_t> regions;
	unsigned int draw_index;

	Vector2 size;
	float up; //determines y position if rendered in 3D
	/* This vector stores all the regions of textures that can be drawn
	 * by this texsprite. Using the same shared_ptr to draw multiple
	 * regions should be fine, because it will automatically handle
	 * deletion without having to store multiple copies of the resources.
	 * src_rect is ignored for "world" type drawing
	 */

	//offset position from the given "draw" position
	Vector2 offset;

	Vector3 rotation_axis;
	float rotation_deg;
	//scaling for all types
	float scale = 1.0;

	Texture2D getCurrentTexture();
	void ageTextures();

public:
	//calculates pos in 3D, and dest rect and origin for screen type
	constexpr Rectangle getDestRect(Vector2 pos)
	{
		return (Rectangle){
			pos.x+offset.x,
			pos.y+offset.y,
			size.x,
			size.y
		};
	}
	constexpr Vector2 getOrigin2D()
	{
		return {size.x/2, size.y/2};
	}
	constexpr Vector3 getPos3D(Vector2 pos)
	{
		return (Vector3){
			pos.x+offset.x,
			up,
			pos.y+offset.y
		};
	}

	TexSprite(const uint8_t resource_index,
		std::string name,
		Type type,
		std::vector<uint8_t> regions,
		Vector2 size);
	//same as above, but defaulting to "screen" SpriteType
	TexSprite(const uint8_t resource_index,
		std::string name,
		std::vector<uint8_t> regions,
		Vector2 size);
	//constructor for "world" type where 'regions' is unused
	TexSprite(const uint8_t resource_index,
		std::string name,
		Vector2 size);

	std::string getName() {return region_name;}
	unsigned int getCurrentRegionID() {return regions[draw_index];}
	void setDrawIndex(int tex_index);
	unsigned int getDrawIndex() const {return draw_index;}
	unsigned int getResID() const {return res_id;}
	Type getType() const {return type;}

	void setOffset(Vector2 offset) {this->offset = offset;}
	Vector2 getOffset() const {return offset;}
	void setSize(Vector2 size) {this->size = size;}
	Vector2 getSize() const {return size;}
	void setScale(float scale) {this->scale = scale;}
	float getScale() const {return scale;}
	Vector3 getScale3D() const {return {scale,scale,scale};}
	void setRotation(Vector3 rotation); //as vector3 of pitch,yaw,roll
	Vector3 getRotation();
	Vector3 getRotationAxis() {return rotation_axis;}
	void setRotationDeg(float rotation) {this->rotation_deg = rotation;}
	float getRotationDeg() {return rotation_deg;}

	void drawBillboard(Texture2D atlas,
		Rectangle src_rect,
		Vector2 pos,
		Camera cam);
	void drawWorld(Model model,
		Vector2 pos,
		Camera cam);
	void drawScreen(Texture2D atlas,
		Rectangle src_rect,
		Color color);
};

#endif
