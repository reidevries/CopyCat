#include <raylib.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <memory>

#include "CatConf.hpp"
#include "CatClock.hpp"
#include "Environment.hpp"
#include "MessageList.hpp"
#include "ResMan.hpp"
#include "ViewRenderer.hpp"
#include "JsonComponents.hpp"

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
    //globally disable backface culling
    //we are not loading models in this game
	rlDisableBackfaceCulling();

    ResMan res_man(debug);
    ViewRenderer view(screen_w, screen_h, debug);
    Environment environment(8, 128);
    JsonComponents::saveLevel(environment.getReg(), "test.json");


    SetTargetFPS(60);
	
    if (ReiDV::VERBOSITY >= 3) JsonComponents::test();

    CatClock clk;

    bool waiting_to_load = true;

    // request floor texture be loaded in advance
    res_man.requestTex("tile/floor");
    
    while (!WindowShouldClose()) {
    	clk.tick(GetFrameTime());
    	res_man.loadNextImage();

		if (waiting_to_load && res_man.isTexLoaded("tile/floor")) {
			environment.initLevel(res_man, string("test"));
			waiting_to_load = false;
		}

		view.render(clk, environment.getReg(), res_man);

		res_man.loadNextTex();
    }
    
    CloseWindow();

    return 0;
}
