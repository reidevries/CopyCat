/*
 * ResSprite.cpp
 *
 *  Created on: Aug 11, 2020
 *      Author: rei de vries
 */

#include "ResSprite.hpp"

using namespace std;


void to_json(json& j, const ResSprite& s)
{
	j["ResSprite"] = {
		{"atlas name", s.atlas_name},
		{"region name", s.region_name},
		{"tint", s.tint}
	};
}

void from_json(const json& j, ResSprite& s)
{
	const json& sprite = j.at("ResSprite");
	sprite.at("atlas name").get_to(s.atlas_name);
	sprite.at("region name").get_to(s.region_name);
	sprite.at("tint").get_to(s.tint);
}
