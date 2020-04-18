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

#ifndef SRC_TEXREGION_H_
#define SRC_TEXREGION_H_

#include <raylib.h>
#include <iostream>
#include "debugprinter.h"
#include <memory>
#include "texres.h"

class TexRegion {
	private:
		std::shared_ptr<TexRes> tex_res;
		Rectangle src_rect;
	public:
		TexRegion(std::shared_ptr<TexRes> tex_res, Rectangle src_rect);
		TexRegion(std::shared_ptr<TexRes> tex_res);
		Rectangle getSrcRect() {return src_rect;}

		//passthru methods
		Texture2D get() {return tex_res->get();} //get the underlying Texture2D
		std::shared_ptr<TexRes> getRes() {return tex_res;} //get the pointer to the resource
		void age(float dt) {tex_res->age(dt);} //ages the resource
		void seen() {tex_res->seen();} //resets the age of the resource
};


#endif /* SRC_TEXREGION_H_ */
