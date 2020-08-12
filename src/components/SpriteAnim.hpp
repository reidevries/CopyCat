/*
 * SpriteAnim.h
 *
 *  Created on: Jul 17, 2020
 *      Author: rei de vries
 *		Component representing the frames of an animation.
 *		Also used for sprites with no animation by setting
 *		num_frames = 0
 */

#ifndef SRC_COMPONENTS_SPRITEANIM_H_
#define SRC_COMPONENTS_SPRITEANIM_H_

#include <array>
#include "../ResSprite.hpp"
#include "CatConf.hpp"

struct SpriteAnim
{
	ResSprite sprite;
	bool show = true;
	float anim_speed = 0.0f; //in fps
	float frame_s;	//seconds since last frame changed
};



#endif /* SRC_COMPONENTS_SPRITEANIM_H_ */
