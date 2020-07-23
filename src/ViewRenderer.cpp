/*
 * viewrenderer.cpp
 *
 *  Created on: Jun 13, 2020
 *      Author: rei de vries
 */

#include "ViewRenderer.h"

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

	Image checked = LoadImage("sprite/tile/floor_dirt.png");
	Texture2D texture = LoadTextureFromImage(checked);
	UnloadImage(checked);

	testmodel = LoadModelFromMesh(
		static_cast<Mesh>(GenMeshPlane(211.0, 211.0, 1, 1))
	);
	testmodel.materials[0].maps[MAP_ALBEDO].texture = texture;
}

void ViewRenderer::render(CatClock& clk,
	entt::registry& reg,
	ResMan& resman)
{
	BeginDrawing();
	ClearBackground(RAYWHITE);

	const auto render_list = reg.view<SpriteAnim, WorldPos, SpriteQuad>();

	BeginMode3D(cam);
	//DrawModel(testmodel, {0,0}, 1.0f, WHITE);

	for (const entt::entity e : render_list) {
		const SpriteAnim sprite = render_list.get<SpriteAnim>(e);
		ReiDV::drawQuad( resman.getTexAt(sprite.res_id),
			resman.getRegionAt(sprite.res_id, sprite.getCurRegion()),
			render_list.get<WorldPos>(e).pos,
			render_list.get<SpriteQuad>(e).quad );
	}

	if (debug) {
		DrawGrid(1000, METRE);
	}

	EndMode3D();

	if (debug) renderDebug(clk, reg, resman);
	EndDrawing();
}

//call between BeginMode3D and EndMode3D
void ViewRenderer::renderAxes()
{
	DrawLine3D(
		(Vector3){METRE,METRE,METRE},
		(Vector3) {METRE*2,METRE,METRE},
		RED);
	DrawLine3D(
		(Vector3){METRE,METRE,METRE},
		(Vector3) {METRE,METRE*2,METRE},
		GREEN);
	DrawLine3D(
		(Vector3){METRE,METRE,METRE},
		(Vector3) {METRE,METRE,METRE*2},
		BLUE);
}

//call dis between BeginDrawing() and EndDrawing() but not in 3D
void ViewRenderer::renderAxes2D(Vector2 screen_pos, float cam_rot)
{
	Rectangle line_thick = {screen_pos.x,screen_pos.y,METRE*0.25,METRE*5};
	Vector2 origin = {line_thick.width/2, line_thick.height};
	float font_size = font.baseSize;
	float font_origin_y = font_size/2;
	float font_origin_x = font_size/4;
	Vector2 x_text = (Vector2){
		screen_pos.x + line_thick.height	- font_origin_x,
		screen_pos.y + line_thick.height/2	- font_origin_y
	};
	Vector2 y_text = (Vector2){
		screen_pos.x - font_origin_x,
		screen_pos.y - line_thick.height	- font_size
	};
	Vector2 z_text = (Vector2){
		screen_pos.x - line_thick.height	- font_origin_x,
		screen_pos.y + line_thick.height/2	- font_origin_y
	};

	DrawRectanglePro(line_thick, origin, 120+cam_rot, RED);
	DrawTextEx(font, "X", x_text, font_size, 1.0f, RED);
	DrawRectanglePro(line_thick, origin, 0, GREEN);
	DrawTextEx(font, "Y", y_text, font_size, 1.0f, GREEN);
	DrawRectanglePro(line_thick, origin, 240+cam_rot, BLUE);
	DrawTextEx(font, "Z", z_text, font_size, 1.0f, BLUE);
}

//call dis between BeginDrawing() and EndDrawing() but not in 3D
void ViewRenderer::renderDebug(CatClock& clk,
	entt::registry& reg,
	ResMan& resman)
{
	stringstream debugtxt;
	debugtxt << "fps: " << clk.fps() << "\n";

	const auto view = reg.view<SpriteAnim>();
	for (const entt::entity e : view) {
		debugtxt << view.get<SpriteAnim>(e).region_name << "\n";
	}

	Color c = VectorMath::hsvToRgb(128, 222, 250, 128);
	DrawRectangle(screen_w * 0.7, screen_h * 0.1, screen_w * 0.25,
		screen_h * 0.8, c);
	// Draw text directly using sprite font
	DrawTextEx(font, debugtxt.str().c_str(), (Vector2 ) {
			static_cast<float>(screen_w * 0.7),
			static_cast<float>(screen_h * 0.1)
		}, static_cast<float>(font.baseSize * 0.75), 2.0f, RED);

	renderAxes2D(
		(Vector2){
			static_cast<float>(screen_w*0.1),
			static_cast<float>(screen_h*0.9)
		}, 0);
}
