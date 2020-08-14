/*
 * Created: 02/08 2020
 *  by Rei de Vries
 */

#include "ManAudio.hpp"

using namespace std;

const string ManAudio::audio_directory = "audio/";

string ManAudio::constructAudioFilename(const string& name) const
{
	return audio_directory + name + ".ogg";
}

void ManAudio::freeAudioByIndex(const size_t index)
{
	UnloadSound(audio_buf.pop(index));
}

ManAudio::ManAudio(const bool set_debug) : debug(set_debug) {}

size_t ManAudio::requestAudio(const string& name)
{
	size_t audio_id = Res::AUDIO_BUF_SIZE;
	if (audio_buf.has(name)) {
		audio_id = audio_buf.find(name);
	} else {
		audio_id = audio_buf.push(name, Sound());
		audio_load_queue.push(name);
	}
	return audio_id;
}

size_t ManAudio::getAudioID(const string& name)
{
	size_t  audio_id = Res::AUDIO_BUF_SIZE;
	if (audio_buf.has(name)) {
		audio_id = audio_buf.find(name);
	}
	return audio_id;
}

void ManAudio::loadNextAudio()
{
	if (audio_load_queue.empty()) return;
	 
	string to_load = audio_load_queue.front();
	size_t cur_id = 0;
	try {
		cur_id = audio_buf.find(to_load);
	} catch (const std::out_of_range& e) {
		stringstream ss;
		ss << to_load << " should've been initialised, but hasn't";
		DebugPrinter::printDebug(0, "ManAudio::loadNextAudio", ss.str());
		return;
	}
	stringstream ss;
	ss << "loading " << to_load 
		<<  " into buf at index " << static_cast<int>(cur_id);
	DebugPrinter::printDebug(2, "ManAudio::loadNextAudio", ss.str());
	
	string audio_filename = constructAudioFilename(to_load);
	if (FileExists(audio_filename.c_str())) {
		audio_buf.replace(cur_id, LoadSound(audio_filename.c_str()));
	} else {
		ss.str("");
		ss << "ERROR: no such file as " << audio_filename;
		DebugPrinter::printDebug(0, "ManAudio::loadNextAudio", ss.str());
	}
	
	ss.str("");
	Sound s = audio_buf.get(cur_id);
	ss << "loaded sound looks like this: " << endl
	<< "sampleCount: " << s.sampleCount << endl 
	<< "sampleRate: " << s.stream.sampleRate << endl
	<< "sampleSize: " << s.stream.sampleSize << endl
	<< "channels: " << s.stream.channels << endl
	<< "buffer pointer: " <<  s.stream.buffer << endl;
	DebugPrinter::printDebug(4, "ManAudio::loadNextAudio", ss.str());

	audio_load_queue.pop();
}

bool ManAudio::isAudioLoaded(const size_t id) const
{
	if (!audio_buf.isFree(id)) {
		if (audio_buf.get(id).stream.buffer != nullptr) {
			return true;
		}
	}
	return false;
}

bool ManAudio::isAudioLoaded(const string& name) const
{
	size_t id = audio_buf.find(name);
	return isAudioLoaded(id);
}

Sound& ManAudio::getAudioAt(const size_t id)
{
	return audio_buf.at(id);
}

Sound& ManAudio::getAudioAt(const string& name)
{
	return audio_buf.at(name);
}

void ManAudio::playSound(const ResSound& sound, 
	const float vol, 
	const float pitch)
{
	if (sound.res_id < Res::AUDIO_BUF_SIZE) {
		Sound s = getAudioAt(sound.res_id);
		SetSoundPitch(s, sound.pitch*pitch);
		SetSoundVolume(s, sound.vol*vol);
		PlaySound(s);
	} else {
		stringstream ss;
		ss << "invalid res_id of sound with name " << sound.name;
		throw out_of_range(ss.str());
	}
}

void ManAudio::playSound(const ResSound& sound, const float vol)
{
	playSound(sound, vol, 1);
}

void ManAudio::playSound(const ResSound& sound)
{
	playSound(sound,1,1);
}

void ManAudio::playSoundMulti(const ResSound& sound)
{
	if (sound.res_id < Res::AUDIO_BUF_SIZE) {
		Sound s = getAudioAt(sound.res_id);
		SetSoundVolume(s, sound.vol);
		PlaySoundMulti(s);
	} else {
		stringstream ss;
		ss << "invalid res_id of sound with name " << sound.name;
		throw out_of_range(ss.str());
	}
}

ResSound ManAudio::constructSound(const string& name)
{
	ResSound s(name);
	s.res_id = requestAudio(name);
	s.pitch = 1.0f;
	s.vol = 1.0f;
	return s;
}

ManAudio::~ManAudio()
{
	for (auto& sound : audio_buf.popAll()) {
		if ( sound.stream.buffer != nullptr ) UnloadSound(sound);
	}
}
