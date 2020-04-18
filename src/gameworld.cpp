#include "gameworld.h"
#include <sstream>
#include <iostream>

GameWorld::GameWorld(int screen_w, int screen_h, bool debug) {
	this->debug = debug;
	this->screen_w = screen_w;
	this->screen_h = screen_h;
	screen_scale = screen_h/WORLD_H;
	world_w = WORLD_H*(screen_w/screen_h);

	font = LoadFontEx("font/Andika/Andika-R.ttf", int(6*screen_scale), 0, 0);
	
	current_level = std::make_unique<Level>();

	Polygon v((Vector2){20,20}, 7, 69);
	mersenne_twister = std::mt19937(123);
	distribute = std::uniform_real_distribution<double>(0.0,1.0);
	if (debug) std::cout << "screen scale set to " << screen_scale << " i am happy" << std::endl;
}

void GameWorld::ageTextures(float dt) {

}

std::unique_ptr<ResMan> GameWorld::update(float dt, unsigned int time_s, std::unique_ptr<ResMan> resman) {
	if (ui_buf.size() <= 1 || IsMouseButtonDown(0)) {
		std::unique_ptr<TexSprite> tex_face = resman->createTextureSprite("face", screen_scale);
		tex_face->setOffset((Vector2){distribute(mersenne_twister)*WORLD_H,distribute(mersenne_twister)*WORLD_H});
		tex_face->setScale(distribute(mersenne_twister));
		tex_face->setRotation(distribute(mersenne_twister)*360);
		ui_buf.push_back(std::move(tex_face));
		DebugPrinter::printDebug("GameWorld::update", "created face texture at time " + std::to_string(time_s), 1);
	}
	for (auto&& ui : ui_buf) {
		ui->setRotation(ui->getRotation()+1);
	}
	for (auto&& object : object_buf) {
		if (object.get() != nullptr) {
			current_level = object->update(std::move(current_level), dt);
		} else {
			DebugPrinter::printDebug("GameWorld::update", reinterpret_cast<std::uintptr_t>(object.get()), " is not an object?", 0);
		}
	}
	return std::move(resman);
}

void GameWorld::render(float dt) {
	BeginDrawing();

	ClearBackground(RAYWHITE);
	for (auto&& object : object_buf) {
		object->draw();
	}
	
	for (auto&& ui : ui_buf) {
		ui->draw(WHITE);
	}

	if (debug) renderDebug(dt);
	EndDrawing();
}

void GameWorld::renderDebug(float dt) { //call dis between BeginDrawing() and EndDrawing()
	float fps = 1.0/dt;

	std::stringstream debugtxt;
	debugtxt << "fps: " << fps << "\nui sprites: " << ui_buf.size();
	Color c = VectorMath::hsvToRgb(128, 222, 250, 128);
	DrawRectangle(screen_w*0.7, screen_h*0.1, screen_w*0.25, screen_h*0.8, c);
	// Draw text directly using sprite font
	DrawTextEx(font,
		debugtxt.str().c_str(),
		(Vector2){
			static_cast<float>(screen_w*0.7),
			static_cast<float>(screen_h*0.1)
		},
		static_cast<float>(font.baseSize*1.8), 2.0f, RED
	);

}

GameWorld::~GameWorld() {
	object_buf.clear();
	UnloadFont(font);           // Unload custom spritefont
}
