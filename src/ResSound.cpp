/*
 * Created: 02/08 2020
 *  by Rei de Vries
 */

#include "ResSound.hpp"

using namespace std;

ResSound::ResSound() : res_id(Res::AUDIO_BUF_SIZE) {}

ResSound::ResSound(string set_name)
	:  res_id(Res::AUDIO_BUF_SIZE), name(set_name)
{
}

void to_json(json& j, const ResSound& s)
{
	j["ResSound"] = {
		{"name",  s.name}, 
		{"pitch", s.pitch}, 
		{"vol", s.vol}
	};
}

void from_json(const json& j, ResSound& s)
{
	const json& res_sound = j.at("ResSound");
	res_sound.at("name").get_to(s.name);
	res_sound.at("pitch").get_to(s.pitch);
	res_sound.at("vol").get_to(s.vol);
}
