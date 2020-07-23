#include <raylib.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <memory>

#include "CatConf.h"
#include "CatClock.h"
#include "Environment.h"
#include "MessageList.h"
#include "ResMan.h"
#include "ViewRenderer.h"
#include "JsonComponents.h"

#ifndef CATCONF_H
#define CAT_VERBOSITY 0
#define COPYCAT_VERSION_MAJOR 0
#define COPYCAT_VERSION_MINOR 0
#endif

using namespace std;

const bool debug = (CAT_VERBOSITY > 0);

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
    environment.initLevel(res_man, string("test"));

    SetTargetFPS(60);
	
    JsonComponents::test();

    CatClock clk;

    bool waiting_to_load_test_region = true;

    while (!WindowShouldClose()) {
    	clk.tick(GetFrameTime());
    	res_man.loadNextImage();

		if (waiting_to_load_test_region) {
			waiting_to_load_test_region = false;
		}

		view.render(clk, environment.getReg(), res_man);

		res_man.loadNextTex();
    }
    
    CloseWindow();

    return 0;
}
