/*
 * Created: 02/08 2020
 *  by Rei de Vries
 */

#include "SoundRes.hpp"

using namespace std;

SoundRes::SoundRes() : res_id(Res::AUDIO_BUF_SIZE) {}

SoundRes::SoundRes(string set_name)
	:  res_id(Res::AUDIO_BUF_SIZE), name(set_name)
{
}

void to_json(json& j, const SoundRes& s)
{
	j["SoundRes"] = {
		{"name",  s.name}, 
		{"pitch", s.pitch}, 
		{"vol", s.vol}
	};
}

void from_json(const json& j, SoundRes& s)
{
	const json& sound_res = j.at("SoundRes");
	sound_res.at("name").get_to(s.name);
	sound_res.at("pitch").get_to(s.pitch);
	sound_res.at("vol").get_to(s.vol);
}
