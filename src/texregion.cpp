/*
 * texregion.cpp
 *
 *  Created on: Apr 15, 2020
 *      Author: rei de vries
 */

#include "texregion.h"

TexRegion::TexRegion(std::shared_ptr<TexRes> tex_res, Rectangle src_rect) {
	this->tex_res = tex_res;
	this->src_rect = src_rect;
}

TexRegion::TexRegion(std::shared_ptr<TexRes> tex_res) {
	this->tex_res = tex_res;
	this->src_rect = (Rectangle){
		0,
		0,
		static_cast<float>(tex_res->getWidth()),
		static_cast<float>(tex_res->getHeight())
	};
}
