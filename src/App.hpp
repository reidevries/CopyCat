/*
 * App.hpp
 *
 *  Created on: August 14th, 2020
 *      Author: rei de vries
 * Class representing the game app
 */
 
#ifndef SRC_APP_H_
#define SRC_APP_H_

#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

#include "CatConf.hpp"
#include "CatClock.hpp"
#include "Environment.hpp"
#include "MessageList.hpp"
#include "ManTex.hpp"
#include "ViewRenderer.hpp"
#include "ComponentsJson.hpp"
#include "IncludeSystems.hpp"
#include "InputData.hpp"

class App
{
private:
	const int screen_w;
	const int screen_h;
	const bool debug;
	
	std::stringstream window_title;
	
	ManTex man_tex;
	ManAudio man_audio;
	ViewRenderer view_renderer;
	Environment environment;
	InputData input_data;
	CatClock clk;
	Systems systems;
public:
	App(const int w, const int h, const bool set_debug);
	void update();
};

#endif
