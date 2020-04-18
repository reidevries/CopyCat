/*
 * textureres.c
 *
 *  Created on: Apr 2, 2020
 *      Author: raeaw
 */

#include "texres.h"

TexRes::TexRes() {
	//generate a blank texture as default
	this->tex = LoadTextureFromImage(GenImageColor(99, 99, YELLOW));
	last_seen = 0;
}

TexRes::TexRes(const char* filename) {
	this->tex = LoadTexture(filename);
	last_seen = 0;
}

TexRes::~TexRes() {
	UnloadTexture(tex);
}



