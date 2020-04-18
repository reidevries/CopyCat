#include <raylib.h>
#include "gameworld.h"
#include "resman.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    const int screen_w = 1280;	//size of viewport in pixels
	const int screen_h = 720;
	
	char* debugenv = getenv("CATDEBUG");
	bool debug = false;
	if (debugenv != NULL) {
		if (strncmp(debugenv, "true", 4) == 0) {
			debug = true;
			std::cout << "DEBUG MODE ON" << std::endl;
		}
	}
	
    InitWindow(screen_w, screen_h, "copycat");
    std::unique_ptr<ResMan> resman = std::make_unique<ResMan>(debug);
    GameWorld gameworld(screen_w, screen_h, debug);
    SetTargetFPS(60);
	
    float counter_ms = 0;	//when this gets to 1000, add a second to time_s
    unsigned long time_s = 0;

    while (!WindowShouldClose()) {
    	float dt = GetFrameTime();
    	counter_ms += dt;
    	if (counter_ms > 1000) {
    		++time_s;
    		counter_ms -= 1000; //subtract by 1000 rather than setting =0, so the time_s doesn't drift too much
    	}
		resman = gameworld.update(dt, time_s, std::move(resman));
		resman->loadTextures(dt);
		resman->deleteUnused();
		gameworld.render(dt);
    }
    
    CloseWindow();              // Close window and OpenGL context

    return 0;
}
