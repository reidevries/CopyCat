/*
 * SpriteMulti.h
 *
 *  Created on: Aug 11, 2020
 *      Author: rei de vries
 */
 
#ifndef SRC_SPRITEMULTI_H_
#define SRC_SPRITEMULTI_H_

#include <array>
#include <bitset>
#include "../ResSprite.hpp"
#include "CatConf.hpp"

template<std::size_t num_sprites>
struct SpriteMulti
{
	std::array<ResSprite,num_sprites> sprite_arr;
	std::array<Vector3,num_sprites> pos_arr;
	std::bitset<num_sprites> visible;
};

#endif /* SRC_SPRITEMULTI_H_ */
