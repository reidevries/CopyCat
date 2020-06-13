#include "gameworld.h"
#include <sstream>
#include <iostream>

using namespace std;

void GameWorld::pointCameraAt(Vector2 coords)
{
	cam.target = { coords.x, 0.0f, coords.y };
	cam.position =
		{ coords.x + cam.fovy, cam.fovy * 0.75f, coords.y + cam.fovy };
}

GameWorld::GameWorld(int screen_w, int screen_h, bool debug)
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

	font = LoadFontEx("font/Andika/Andika-R.ttf", int(6 * screen_scale), 0, 0);

	current_level = make_unique<Level>();

	Polygon v((Vector2) { 20, 20 }, 7, 69);
	mersenne_twister = mt19937(123);
	distribute = uniform_real_distribution<double>(0.0, 1.0);
	if (debug)
		cout << "screen scale set to " << screen_scale << " i am happy"
			<< endl;
}

void GameWorld::ageTextures(float dt)
{

}

unique_ptr<ResMan> GameWorld::update(float dt, unsigned int time_s,
	unique_ptr<ResMan> resman)
{
	if (ui_buf.size() <= 1 || IsMouseButtonDown(0)) {
		unique_ptr<TexSprite> tex_face = resman->createTextureSprite(
			"face", SpriteType::screen);
		tex_face->setOffset((Vector2) {
			round(distribute(mersenne_twister) * world_w / 10.0f) * 100.0f,
			round(distribute(mersenne_twister) * WORLD_H / 10.0f) * 100.0f
		});
		tex_face->setScale(0.5f);
		ui_buf.push_back(move(tex_face));
		DebugPrinter::printDebug("GameWorld::update",
			"created face texture at time " + to_string(time_s), 1);
	}
	for (auto &&ui : ui_buf) {
		ui->setRotation(ui->getRotation() + 1);
	}
	return move(resman);
}

void GameWorld::render(float dt)
{
	BeginDrawing();
	ClearBackground(RAYWHITE);

	BeginMode3D(cam);
	DrawGrid(1000, 10.0f);
	EndMode3D();

	for (auto &&object : object_buf) {
		object->draw();
	}

	for (auto &&ui : ui_buf) {
		ui->drawUI(WHITE);
	}

	if (debug) renderDebug(dt);
	EndDrawing();
}

void GameWorld::renderDebug(float dt)
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

GameWorld::~GameWorld()
{
	object_buf.clear();
	UnloadFont(font);           // Unload custom spritefont
}
