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

#include "CatConf.h"
#include "DebugPrinter.h"
#include "QuadDraw.h"
#include "VectorMath.h"


class TexSprite {
public:
	//defines how the sprite is to be drawn
	enum class Type {
		screen,		//Gets drawn in 2D by drawUI
					//It is drawn on the screen at the pixel coords of
					//"offset" This is a candidate for being split into
					//another class, but I think it would share too many
					//methods

		world,  	//Gets drawn in 3D as a flat plane textured with the
					//sprite. It's drawn in the world at the coordinates
					//specified by dest_rect and "up", which is formed from
					//offset+pos as specified in draw methods

		billboard 	//gets drawn in 3D as a billboard
					//It's drawn in the same way as "world", but it always
					//faces the camera.
	};

private:
	//ID of the Texture2D or Model from ResMan
	//if type==world it references a Model
	//otherwise it's a Texture2D
	uint8_t res_id;
	std::vector<uint8_t> region_ids; //references which region is being used
	uint8_t anim_index;//references the animation frame of the region name
	std::string region_name;
	Type type;
	Cat::Quad quad;

	Vector2 size;
	float up; //determines y position if rendered in 3D

	//offset position from the given "draw" position
	Vector2 offset;

	Vector3 rot_axis;
	float rot_deg;
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

	TexSprite(const uint8_t res_index,
		std::vector<uint8_t> region_indices,
		std::string name,
		Type type,
		Vector2 size);

	std::string getName() {return region_name;}
	uint8_t getResID() const {return res_id;}
	uint8_t getCurRegionID() const {return region_ids[anim_index];}
	void setAnimIndex(uint8_t anim_index);
	uint8_t getAnimIndex() const {return anim_index;}
	Type getType() const {return type;}

	void setOffset(Vector2 offset) {this->offset = offset;}
	Vector2 getOffset() const {return offset;}
	void setSize(Vector2 size) {this->size = size;}
	Vector2 getSize() const {return size;}
	void setScale(float scale) {this->scale = scale;}
	float getScale() const {return scale;}
	Vector3 getScale3D() const {return {scale,scale,scale};}
	void setRot(Vector3 rotation); //as vector3 of pitch,yaw,roll
	Vector3 getRot();
	Vector3 getRotAxis() {return rot_axis;}
	void getRotDeg(float rotation) {this->rot_deg = rotation;}
	float getRotDeg() {return rot_deg;}

	void rotQuad(float roll, float pitch, float yaw);

	void drawBillboard(Texture2D atlas,
		Rectangle src_rect,
		Vector2 pos,
		Camera cam);
	void drawWorld(Texture2D atlas,
		Rectangle src_rect,
		Vector2 pos,
		Camera cam);
	void drawScreen(Texture2D atlas,
		Rectangle src_rect,
		Color color);
};

#endif
