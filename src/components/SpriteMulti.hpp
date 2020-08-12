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
#include "../ResConstants.hpp"
#include "CatConf.hpp"

struct SpriteMulti
{
public:
	std::array<ResSprite, Res::MAX_MULTI_SPRITES> sprites;
	std::array<Vector3, Res::MAX_MULTI_SPRITES> pos;
	std::array<bool, Res::MAX_MULTI_SPRITES> visible;
	
private:
	int length = 0; //number of sprites used
	
public:
	SpriteMulti() {}
	SpriteMulti(ResSprite a, ResSprite b)
		: sprites({a,b}),
		pos({Vector3{0,0,0}, Vector3{0,0,0}}),
		visible({true,true}),
		length(2)
	{
	}
	
	bool push(const ResSprite& new_sprite, const Vector3& new_pos)
	{
		if (length == Res::MAX_MULTI_SPRITES) return false;
		sprites[length] = new_sprite;
		pos[length] = new_pos;
		visible[length] = true;
		++length;
		return true;
	}
	
	bool getTo(const std::size_t index, 
		ResSprite& empty_sprite, 
		Vector3 empty_vec) const
	{
		if (visible[index] && index < length) {
			empty_sprite = sprites[index];
			empty_vec = pos[index];
			return true;
		} else return false;
	}
	
	std::size_t size() const { return length; }
};

#endif /* SRC_SPRITEMULTI_H_ */
