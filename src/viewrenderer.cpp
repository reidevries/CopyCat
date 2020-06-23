/*
 * viewrenderer.cpp
 *
 *  Created on: Jun 13, 2020
 *      Author: rei de vries
 */

#include "viewrenderer.h"

using namespace std;

void ViewRenderer::pointCameraAt(const Vector2 coords)
{
	cam.target = { coords.x, 0.0f, coords.y };
	cam.position =
		{ coords.x + cam.fovy, cam.fovy * 0.75f, coords.y + cam.fovy };
}

Rectangle ViewRenderer::getCameraFrustrum()
{
	return {
		cam.position.x - cam.fovy,
		cam.position.y - cam.fovy,
		cam.fovy*2,
		cam.fovy*2
	};
}

ViewRenderer::ViewRenderer(const int screen_w, const int screen_h,
	const bool set_debug) : debug(set_debug)
{
	this->screen_w = screen_w;
	this->screen_h = screen_h;
	screen_scale = screen_h / WORLD_H;
	world_w = WORLD_H * (screen_w / screen_h);

	cam = { };
	cam.up = { 0.0f, 1.0f, 0.0f };
	cam.fovy = world_w;
	cam.type = CAMERA_ORTHOGRAPHIC;
	pointCameraAt( { world_w / 10.0f, WORLD_H / 10.0f });

	font = LoadFontEx("font/Andika/Andika-R.ttf",
		int(6 * screen_scale), 0, 0);

	if (debug)
		cout << "screen scale set to " << screen_scale << " i am happy"
			<< endl;

	Image checked = (Image)LoadImage("sprite/tile/floor_dirt.png");
	Texture2D texture = LoadTextureFromImage(checked);
	UnloadImage(checked);

	testmodel = LoadModelFromMesh(
		static_cast<Mesh>(GenMeshPlane(211.0, 211.0, 1, 1))
	);
	testmodel.materials[0].maps[MAP_ALBEDO].texture = texture;
}

void ViewRenderer::render(CatClock& clk, Environment& environment)
{
	BeginDrawing();
	ClearBackground(RAYWHITE);

	multimap<int, shared_ptr<GameObject>> render_list
		= environment.getObjectsInBoxForRender(getCameraFrustrum());

	//set<shared_ptr<GameObject>> render_list
	//	= environment.getAllObjects();

	BeginMode3D(cam);
	DrawGrid(1000, 10.0f);
	for (auto& object : render_list) {
		object.second->draw(cam);
	}

	DrawModel(testmodel, {0,0}, 1.0f, WHITE);

	EndMode3D();

	for (auto &&ui : ui_buf) {
		ui->drawUI(WHITE);
	}

	if (debug) renderDebug(clk, environment);
	EndDrawing();
}

void ViewRenderer::renderDebug(CatClock& clk, Environment& environment)
{ //call dis between BeginDrawing() and EndDrawing()
	stringstream debugtxt;
	debugtxt << "fps: " << clk.fps() << "\n"
		<< "ui sprites: " << ui_buf.size() << "\n";

	for (auto const& object : environment.getAllObjects()) {
		debugtxt << object->getInfo() << "\n";
	}

	Color c = VectorMath::hsvToRgb(128, 222, 250, 128);
	DrawRectangle(screen_w * 0.7, screen_h * 0.1, screen_w * 0.25,
		screen_h * 0.8, c);
	// Draw text directly using sprite font
	DrawTextEx(font, debugtxt.str().c_str(), (Vector2 ) {
			static_cast<float>(screen_w * 0.7),
			static_cast<float>(screen_h * 0.1)
		}, static_cast<float>(font.baseSize * 0.75), 2.0f, RED);

}
