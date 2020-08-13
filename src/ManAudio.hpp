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
#include "ResSound.hpp"

class ManAudio
{
private:
	static const std::string audio_directory;
	std::string constructAudioFilename(const std::string&) const;
	
	const bool debug;
	
	std::queue<std::string> audio_load_queue;
	
	ResBuf<Sound, Res::AUDIO_BUF_SIZE> audio_buf;
	
	std::array<float, Res::AUDIO_BUF_SIZE> old_pitches;
	
	void freeAudioByIndex(const std::size_t index);

public:
	ManAudio(const bool set_debug);
	
	std::size_t requestAudio(const std::string& name);
	std::size_t getAudioID(const std::string& name);
	
	void loadNextAudio();
	
	bool isAudioLoaded(const std::size_t id) const;
	bool isAudioLoaded(const std::string& name) const;
	
	Sound& getAudioAt(const std::size_t id);
	Sound& getAudioAt(const std::string& name);
	
	void playSound(const ResSound& sound, const float vol);
	void playSound(const ResSound& sound) { playSound(sound,1); }
	void playSoundMulti(const ResSound& sound);
	
	ResBuf<Sound, Res::AUDIO_BUF_SIZE>& getAudioBuf() {return audio_buf;}
	
	ResSound constructSound(const std::string& name);
	
	~ManAudio();
};

#endif // MANAUDIO_H
