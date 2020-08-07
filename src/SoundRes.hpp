/*
 * Created: 02/08 2020
 *  by Rei de Vries
 * Stores pitch and volume params for a sound
 */

#ifndef SOUNDRES_HPP_
#define SOUNDRES_HPP_

#include "CatConf.hpp"
#include "IncludeJson.hpp"
#include "ResConstants.hpp"
#include "ResBuf.hpp"

class SoundRes
{
public:
	SoundRes();
	SoundRes(std::string set_name);
	
	//Id of the resource represented by the sound
	//If sound is not yet loaded, =Res::AUDIO_BUF_SIZE
	uint8_t res_id;
	//Name of the resource represented by the sound
	std::string name;
	//Pitch and volume params for playing the sound
	float pitch;
	float vol;
};

void to_json(json& j, const SoundRes& s);
void from_json(const json& j, SoundRes& s);

#endif // SOUNDRES_H
