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

class TexSprite {
	private:
		std::string name;
		/* This vector stores all the regions of textures that can be drawn
		 * by this texturesprite. Using the same shared_ptr to draw multiple
		 * regions should be fine, because it will automatically handle
		 * deletion without having to store multiple copies of the resources.
		 */
		std::vector<TexRegion> regions;
		int draw_index;					//the index of the texture that is currently being drawn
		Vector2 offset;
		Vector2 origin;					//the origin for rotation and positioning 0.0x0.0 = top left corner, 1.0x1.0=bottom right corner
		float scale = 1;
		float rotation = 0;
		
		//screen_scale is the ratio of game units to pixels, for getting the position on the screen
		//pos_pixels and scale_pixels are updated when the scale, pos or screen_scale are changed
		float screen_scale = 1;

		Vector2 origin_pixels;
		Vector2 offset_pixels;
		float scale_pixels = 1;
		Rectangle dest_rect;
		void updatePixels(); //call this when any of the variables offset, origin, scale, or screen_scale are modified
		void updateRectanglePos(Vector2 pos);	//updates the rectangle where the object will be drawn on the screen
		
		Texture2D getCurrentTexture();
		void ageTextures();
	public:
		TexSprite(std::string name, std::vector<TexRegion> regions, float screen_scale);
		std::vector<TexRegion> getAllTextures() {return regions;}
		std::string getName() {return name;}
		
		void setDrawIndex(int draw_index);
		int getDrawIndex() {return draw_index;}
		void setOffset(Vector2 offset);
		Vector2 getOffset() {return offset;}
		void setOrigin(Vector2 origin);
		Vector2 getOrigin() {return origin;}
		void setScale(float scale);
		float getScale() {return scale;}
		void setRotation(float rotation) {this->rotation = rotation;}
		float getRotation() {return rotation;}

		void setScreenScale(float screenscale);
		
		void draw(Color color);
		void draw(Vector2 pos);
		void draw();
};

#endif
