/*
 * texres.h
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
	unsigned short last_seen_s = 0;	//in seconds since last seen on the screen
public:
	TexRes();
	TexRes(const char* filename);
	Texture2D get() {return tex;}

	void reloadTexture(Image& image);

	//passthru methods for texture2D struct
	unsigned int getID() const {return tex.id;}
	int getWidth() const {return tex.width;}
	int getHeight() const {return tex.height;}
	int getMipMaps() const {return tex.mipmaps;}
	int getFormat() const {return tex.format;}

	void age() {++last_seen_s;} //call every time one second passes
	void seen() {last_seen_s = 0;}
	unsigned short getAge() const {return last_seen_s;}
	~TexRes();
};

#endif /* SRC_TEXRES_H_ */
