/*
 * ResSprite.h
 *
 *  Created on: Jul 17, 2020
 *      Author: rei de vries
 *		Struct representing a texture resource to be used by the game,
 *		optionally with an animation. If no animation desired, set
 * 		num_frames = 0 or 1
 */
 
 #ifndef SRC_COMPONENTS_RESSPRITE_H_
#define SRC_COMPONENTS_RESSPRITE_H_

#include <array>
#include <cstdint>
#include <string>

#include "ResConstants.hpp"
#include "CatConf.hpp"
#include "IncludeJson.hpp"

struct ResSprite
{
	//name of the atlas
	std::string atlas_name;

	//name of the region
	std::string region_name;
	
	Color tint = WHITE;

	//id of the atlas containing the animation
	std::size_t res_id;

	//id of the regions that make up the animation
	std::array<std::size_t, Res::MAX_ANIM_FRAMES> region_ids;
	//current index for region_ids
	std::size_t anim_index;
	//represents how many frames are used in the animation
	//must be < Res::MAX_ANIM_FRAMES
	std::size_t num_frames;

	ResSprite()
		: res_id(Res::MAX_ANIM_FRAMES),
		anim_index(0),
		num_frames(0)
	{
		for (auto& id : region_ids) id = Res::MAX_ANIM_FRAMES;
	}

	std::size_t getCurRegion() const
	{
		return region_ids[anim_index];
	}
};

void to_json(json& j, const ResSprite& s);
void from_json(const json& j, ResSprite& s);


#endif /* SRC_COMPONENTS_RESSPRITE_H_ */
