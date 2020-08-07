#include <raylib.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <memory>

#include "CatConf.hpp"
#include "CatClock.hpp"
#include "Environment.hpp"
#include "MessageList.hpp"
#include "ManTex.hpp"
#include "ViewRenderer.hpp"
#include "ComponentsJson.hpp"
#include "IncludeSystems.hpp"
#include "InputData.hpp"

#ifndef CATCONF_H
#define CAT_VERBOSITY 0
#define COPYCAT_VERSION_MAJOR 0
#define COPYCAT_VERSION_MINOR 0
#endif

using namespace std;

const bool debug = (ReiDV::VERBOSITY > 0);

void parseSystemMessages(vector<Message> messages) {
	for (auto const& message: messages) {
		cout << "Message from " << message.src_id
			<< " : " << message.message << endl;
	}
}

int main(int argc, char* argv[])
{
    const int screen_w = 1280;	//size of viewport in pixels
	const int screen_h = 720;

	stringstream window_title;
	window_title << "copycat version "
		<< int(COPYCAT_VERSION_MAJOR)
		<< "." << int(COPYCAT_VERSION_MINOR);

    InitWindow(screen_w, screen_h, window_title.str().c_str());
    InitAudioDevice();
    //globally disable backface culling
    //we are not loading models in this game
	rlDisableBackfaceCulling();

    ManTex man_tex(debug);
    ManAudio man_audio(debug);
    ViewRenderer view_renderer(screen_w, screen_h, debug);
    Environment environment;
    ComponentsJson::saveLevel(environment.getReg(), "test.json");
	InputData input_data;

    SetTargetFPS(60);
	
    if (ReiDV::VERBOSITY >= 3) ComponentsJson::test();

    CatClock clk;

    bool waiting_to_load = true;
    
    while (!WindowShouldClose()) {
    	clk.tick(GetFrameTime());
    	man_tex.loadNextImage();
		input_data.updateValues(view_renderer.getCam());

		if (waiting_to_load) {
			environment.initLevel(man_tex, man_audio, string("test"));
			waiting_to_load = false;
		}

		// system update methods
		Systems::soundOnHover(environment.getReg(), 	
			clk, 
			input_data.getMouseData(), 
			man_audio);
		// end of system update methods
		
		// this is still technically a system update method, but
		// it should definitely go last
		view_renderer.render(clk, environment.getReg(), man_tex);
		
		man_audio.loadNextAudio();
		man_tex.loadNextTex();
    }
    
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
