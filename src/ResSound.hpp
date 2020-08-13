/*
 * Created: 02/08 2020
 *  by Rei de Vries
 * Stores pitch and volume params for a sound
 */

#ifndef RESSOUND_HPP_
#define RESSOUND_HPP_

#include "CatConf.hpp"
#include "IncludeJson.hpp"
#include "ResConstants.hpp"
#include "ResBuf.hpp"

class ResSound
{
public:
	ResSound();
	ResSound(std::string set_name);
	
	//Id of the resource represented by the sound
	//If sound is not yet loaded, =Res::AUDIO_BUF_SIZE
	std::size_t res_id;
	//Name of the resource represented by the sound
	std::string name;
	//Pitch and volume params for playing the sound
	float pitch;
	float vol;
};

void to_json(json& j, const ResSound& s);
void from_json(const json& j, ResSound& s);

#endif // RESSOUND_H
