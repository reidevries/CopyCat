#include "App.hpp"
#include "CatConf.hpp"

#ifndef CATCONF_H
#define CAT_VERBOSITY 0
#define COPYCAT_VERSION_MAJOR 0
#define COPYCAT_VERSION_MINOR 0
#endif

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

using namespace std;

const bool debug = (ReiDV::VERBOSITY > 0);
App app(1920,1080,debug);

/*void updateFrame()
{
	if (IsKeyPressed(KEY_F5)) ToggleFullscreen(); 
	app.view_renderer.updateScreenWH(GetScreenWidth(), GetScreenHeight());

	app.clk.tick(GetFrameTime());
	app.man_tex.loadNextImage();
	app.input_data.updateValues(app.view_renderer.getCam());

	// system update methods
	entt::registry& reg = app.environment.getReg();
	app.systems.soundOnHover(reg, app.input_data.getMouseData(), app.man_audio);
	app.systems.growOnPress(reg, app.clk, app.input_data.getMouseData());
	app.systems.playDroneSound(reg, app.clk, app.man_audio);
	app.systems.velocity(reg, app.clk);
	app.systems.batAI(reg, app.clk, Level::genLattice(), app.man_audio);
	app.systems.spawnCreatures(reg, app.clk, app.man_tex, app.man_audio);
	// end of system update methods
	
	// this is still technically a system update method, but
	// it should definitely go last
	app.view_renderer.render(app.clk, app.environment.getReg(), app.man_tex);
	
	app.man_audio.loadNextAudio();
	app.man_tex.loadNextTex();
}*/

void updateFrame()
{
	app.update();
}


int main(int argc, char* argv[])
{
	

#if defined(PLATFORM_WEB)
	emscripten_set_main_loop(&updateFrame, 0, 1);
#else
	SetTargetFPS(60);
	while (!WindowShouldClose()) // Detect window close button or ESC key
	{
		if (IsKeyPressed(KEY_F5)) ToggleFullscreen(); 
		app.updateScreenWH(GetScreenWidth(), GetScreenHeight());
		updateFrame();
	}
#endif
	return 0;
}
