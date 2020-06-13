#include <raylib.h>
#include "viewrenderer.h"
#include "resman.h"
#include "environment.h"
#include "messagelist.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>

//this is generated by CMake, feel free to remove and edit the defines below
#include "catconf.h"

#ifndef CATCONF_H
#define CATDEBUG 2
#define COPYCAT_VERSION_MAJOR 0
#define COPYCAT_VERSION_MINOR 0
#endif

int main(void)
{
    const int screen_w = 1280;	//size of viewport in pixels
	const int screen_h = 720;
	
	bool debug = (CATDEBUG > 0);

	std::stringstream window_title;
	window_title << "copycat version "
		<< int(COPYCAT_VERSION_MAJOR)
		<< "." << int(COPYCAT_VERSION_MINOR);

    InitWindow(screen_w, screen_h, window_title.str().c_str());

    ResMan resman(debug);
    ViewRenderer view(screen_w, screen_h, debug);
    Environment environment(8, 128);

    SetTargetFPS(60);
	
    float counter_ms = 0;
    unsigned long time_s = 0;

    while (!WindowShouldClose()) {
    	float dt = GetFrameTime();
    	counter_ms += dt;
    	if (counter_ms > 1000) {
    		++time_s;
    		//subtract by 1000 rather than setting =0,
    		//so the time_s doesn't drift too much
    		counter_ms -= 1000;
    	}
		environment.update(dt, time_s, resman);
		resman.loadTextures(dt);
		resman.deleteUnused();
		view.render(dt, environment);
    }
    
    CloseWindow();

    return 0;
}
