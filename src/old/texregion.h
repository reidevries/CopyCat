/*
 * texregion.h
 *
 *  Created on: Apr 15, 2020
 *      Author: rei de vries
 *	This class represents a region of a texture resource
 *	to be drawn on the screen. src_rect is the portion
 *	of the texture that will be drawn, dest_rect is the
 *	quadrilateral used to draw on the screen, which can
 *	be used to do transformations and stuff.
 */

#ifndef SRC_OLD_TEXREGION_H_
#define SRC_OLD_TEXREGION_H_

#include <raylib.h>
#include <iostream>
#include <memory>

#include "../DebugPrinter.h"
#include "old/texres.h"

class TexRegion {
private:
	std::shared_ptr<TexRes> tex_res;
	Rectangle src_rect;
public:
	TexRegion(std::shared_ptr<TexRes> tex_res, Rectangle src_rect);
	TexRegion(std::shared_ptr<TexRes> tex_res);
	Rectangle getSrcRect() const {return src_rect;}
	//get the dimensions of the src rect
	Vector2 getDimensions() const;

	//get the underlying Texture2D
	Texture2D get() {return tex_res->get();}
	//get the pointer to the resource
	std::shared_ptr<TexRes> getRes() const {return tex_res;}
	//resets the age of the underlying resource
	void seen() {tex_res->seen();}
};


#endif /* SRC_OLD_TEXREGION_H_ */
