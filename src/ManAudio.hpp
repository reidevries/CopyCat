/*
 * Created: 02/08 2020
 *  by Rei de Vries
 */

#ifndef MANAUDIO_HPP_
#define MANAUDIO_HPP_

#include <queue>
#include <array>
#include <string>
#include <sstream>
#include <cstdint>

#include "CatConf.hpp"
#include "DebugPrinter.hpp"
#include "ResBuf.hpp"
#include "ResConstants.hpp"
#include "SoundRes.hpp"

class ManAudio
{
private:
	static const std::string audio_directory;
	std::string constructAudioFilename(const std::string&) const;
	
	const bool debug;
	
	std::queue<std::string> audio_load_queue;
	
	ResBuf<Sound, Res::AUDIO_BUF_SIZE> audio_buf;
	
	void freeAudioByIndex(const uint8_t index);

public:
	ManAudio(const bool set_debug);
	
	uint8_t requestAudio(const std::string& name);
	uint8_t getAudioID(const std::string& name);
	
	void loadNextAudio();
	
	bool isAudioLoaded(const uint8_t id) const;
	bool isAudioLoaded(const std::string& name) const;
	
	Sound& getAudioAt(const uint8_t id);
	Sound& getAudioAt(const std::string& name);
	
	ResBuf<Sound, Res::AUDIO_BUF_SIZE>& getAudioBuf() {return audio_buf;}
	
	SoundRes constructSound(const std::string& name);
};

#endif // MANAUDIO_H