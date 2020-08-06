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

void ManAudio::freeAudioByIndex(const uint8_t index)
{
	UnloadSound(audio_buf.pop(index));
}

ManAudio::ManAudio(const bool set_debug) : debug(set_debug) {}

uint8_t ManAudio::requestAudio(const string& name)
{
	uint8_t audio_id = Res::AUDIO_BUF_SIZE;
	if (audio_buf.has(name)) {
		audio_id = audio_buf.find(name);
	} else {
		audio_id = audio_buf.push(name, Sound());
		audio_load_queue.push(name);
	}
	return audio_id;
}

uint8_t ManAudio::getAudioID(const string& name)
{
	uint8_t audio_id = Res::AUDIO_BUF_SIZE;
	if (audio_buf.has(name)) {
		audio_id = audio_buf.find(name);
	}
	return audio_id;
}

void ManAudio::loadNextAudio()
{
	if (audio_load_queue.empty()) return;
	 
	string to_load = audio_load_queue.front();
	uint8_t cur_id = 0;
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
	audio_load_queue.pop();
}

bool ManAudio::isAudioLoaded(const uint8_t id) const
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
	uint8_t id = audio_buf.find(name);
	return isAudioLoaded(id);
}

Sound& ManAudio::getAudioAt(const uint8_t id)
{
	return audio_buf.at(id);
}

Sound& ManAudio::getAudioAt(const string& name)
{
	uint8_t id = audio_buf.find(name);
	return getAudioAt(id);
}

SoundRes ManAudio::constructSound(const string& name)
{
	SoundRes s(name);
	s.res_id = requestAudio(name);
	return s;
}
