/*
 * Created: 14/08 2020
 *  by Rei de Vries
 */

#include "App.hpp"

using namespace std;

App::App(const int w, const int h, const bool set_debug)
	: screen_w(w), screen_h(h), 
		debug(set_debug), 
		man_tex(set_debug), 
		man_audio(set_debug),
		view_renderer(w,h,set_debug)
{
	window_title << "copycat version "
		<< int(COPYCAT_VERSION_MAJOR)
		<< "." << int(COPYCAT_VERSION_MINOR);
		
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(screen_w, screen_h, window_title.str().c_str());
    SetWindowMinSize(320, 240);
    InitAudioDevice();
    //globally disable backface culling
    //we are not loading models in this game
	rlDisableBackfaceCulling();
	
	environment.initLevel(man_tex, man_audio, string("test"));
    if (ReiDV::VERBOSITY >= 3) ComponentsJson::test();
}

void App::update()
{
	clk.tick(GetFrameTime());
	man_tex.loadNextImage();
	input_data.updateValues(view_renderer.getCam());

	// system update methods
	entt::registry& reg = environment.getReg();
	systems.soundOnHover(reg, input_data.getMouseData(), man_audio);
	systems.growOnPress(reg, clk, input_data.getMouseData());
	systems.playDroneSound(reg, clk, man_audio);
	systems.velocity(reg, clk);
	systems.batAI(reg, clk, Level::genLattice(), man_audio);
	systems.spawnCreatures(reg, clk, man_tex, man_audio);
	// end of system update methods
	
	// this is still technically a system update method, but
	// it should definitely go last
	view_renderer.render(clk, environment.getReg(), man_tex);
	
	man_audio.loadNextAudio();
	man_tex.loadNextTex();
}

App::~App()
{
    CloseAudioDevice();
    CloseWindow();
}
