/*
 * viewrenderer.cpp
 *
 *  Created on: Jun 13, 2020
 *      Author: rei de vries
 */

#include "viewrenderer.h"

using namespace std;

void ViewRenderer::pointCameraAt(Vector2 coords)
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
		cam.fovy,
		cam.fovy
	};
}

ViewRenderer::ViewRenderer(int screen_w, int screen_h, bool debug)
{
	this->debug = debug;
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
}

void ViewRenderer::render(float dt, Environment& environment)
{
	BeginDrawing();
	ClearBackground(RAYWHITE);

	BeginMode3D(cam);
	DrawGrid(1000, 10.0f);
	EndMode3D();

	map<int, shared_ptr<GameObject>> render_list
		= environment.getObjectsInBoxForRender(getCameraFrustrum());

	for (auto& object : render_list) {
		object.second->draw();
	}

	for (auto &&ui : ui_buf) {
		ui->drawUI(WHITE);
	}

	if (debug) renderDebug(dt);
	EndDrawing();
}

void ViewRenderer::renderDebug(float dt)
{ //call dis between BeginDrawing() and EndDrawing()
	float fps = 1.0 / dt;

	stringstream debugtxt;
	debugtxt << "fps: " << fps << "\nui sprites: " << ui_buf.size();
	Color c = VectorMath::hsvToRgb(128, 222, 250, 128);
	DrawRectangle(screen_w * 0.7, screen_h * 0.1, screen_w * 0.25,
		screen_h * 0.8, c);
	// Draw text directly using sprite font
	DrawTextEx(font, debugtxt.str().c_str(), (Vector2 ) {
			static_cast<float>(screen_w * 0.7),
			static_cast<float>(screen_h * 0.1)
		}, static_cast<float>(font.baseSize * 1.8), 2.0f, RED);

}
