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
#include <cstdint>
#include <string>

#include "../ResConstants.hpp"

struct SpriteAnim
{
	//name of the atlas
	std::string atlas_name;

	//name of the region
	std::string region_name;

	//id of the atlas containing the animation
	uint8_t res_id;

	//id of the regions that make up the animation
	std::array<uint8_t, Res::MAX_ANIM_FRAMES> region_ids;
	//current index for region_ids
	std::size_t anim_index;
	//represents how many frames are used in the animation
	//must be < Res::MAX_ANIM_FRAMES
	std::size_t num_frames;

	SpriteAnim()
		: res_id(Res::MAX_ANIM_FRAMES),
		anim_index(0),
		num_frames(0)
	{
		for (auto& id : region_ids) id = Res::MAX_ANIM_FRAMES;
	}

	uint8_t getCurRegion() const
	{
		return region_ids[anim_index];
	}
};



#endif /* SRC_COMPONENTS_SPRITEANIM_H_ */
