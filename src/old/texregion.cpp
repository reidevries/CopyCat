/*
 * texregion.cpp
 *
 *  Created on: Apr 15, 2020
 *      Author: rei de vries
 */

#include "old/texregion.h"

using namespace std;

TexRegion::TexRegion(shared_ptr<TexRes> tex_res, Rectangle src_rect)
{
	this->tex_res = tex_res;
	this->src_rect = src_rect;
}

TexRegion::TexRegion(shared_ptr<TexRes> tex_res)
{
	this->tex_res = tex_res;
	this->src_rect = (Rectangle){
		0,
		0,
		static_cast<float>(tex_res->getWidth()),
		static_cast<float>(tex_res->getHeight())
	};
}

Vector2 TexRegion::getDimensions() const
{
	return (Vector2){src_rect.width, src_rect.height};
}
