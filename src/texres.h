/*
 * TextureResource.h
 *
 *  Created on: Mar 23, 2020
 *      Author: rei de vries
 *      This class represents an individual texture resource used by the game,
 *      and stores the time since it was last displayed on screen. This way
 *      we can automatically draw the same texture multiple times on screen
 *      without also loading it multiple times and wasting memory.
 *      Only one of these objects should exist in memory at a time, with the
 *      memory shared by texturesprites that
 *      use the same texture.
 */

#ifndef SRC_TEXRES_H_
#define SRC_TEXRES_H_

#include <raylib.h>
#include <iostream>
#include "debugprinter.h"

class TexRes {
	private:
		Texture2D tex;
		double last_seen;	//in ms since last seen on the screen
	public:
		TexRes();
		TexRes(const char* filename);
		Texture2D get() {return tex;}

		//passthru methods for texture2D struct
		unsigned int getID() {return tex.id;}
		int getWidth() {return tex.width;}
		int getHeight() {return tex.height;}
		int getMipMaps() {return tex.mipmaps;}
		int getFormat() {return tex.format;}

		void age(float dt) {last_seen += dt;}
		void seen() {last_seen = 0;}
		double getAge() {return last_seen;}
		~TexRes();
};

#endif /* SRC_TEXRES_H_ */
