/*
 * texres.cpp
 *
 *  Created on: Apr 2, 2020
 *      Author: rei de vries
 */

#include "texres.h"

//generate a blank texture as default
TexRes::TexRes()
{
	tex = LoadTextureFromImage(GenImageColor(99, 99, YELLOW));
}

//load texture from filename
TexRes::TexRes(const char* filename)
{
	tex = LoadTexture(filename);
}

void TexRes::reloadTexture(Image& image) {
	UpdateTexture(tex, image.data);
}

TexRes::~TexRes() {
	UnloadTexture(tex);
}



