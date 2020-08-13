/*
 * viewrenderer.cpp
 *
 *  Created on: Jun 13, 2020
 *      Author: rei de vries
 */

#include "ViewRenderer.hpp"

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
	screen_scale = screen_h / World::HEIGHT;
	world_w = World::HEIGHT * (screen_w / screen_h);

	cam = { };
	cam.up = { 0.0f, 1.0f, 0.0f };
	cam.fovy = world_w;
	cam.type = CAMERA_ORTHOGRAPHIC;
	pointCameraAt( { world_w / 2.0f, World::HEIGHT / 2.0f });

	font = LoadFontEx("font/Andika/Andika-R.ttf",
		int(6 * screen_scale), 0, 0);

	if (debug)
		cout << "screen scale set to " << screen_scale << " i am happy"
			<< endl;
}

void ViewRenderer::render(CatClock& clk,
	entt::registry& reg,
	ManTex& man_tex)
{
	BeginDrawing();
	ClearBackground(RAYWHITE);

	BeginMode3D(cam);

	//should do this every time a WorldPos component is added or removed,
	//based on an observer pattern. But I'm rushing to get this gamejam
	//done so this is how I'm doing it for now
	reg.sort<WorldPos>([](const auto &lhs, const auto &rhs) {
		return lhs.pos.x+lhs.pos.y < rhs.pos.x+rhs.pos.y;
	});
	reg.sort<SpriteAnim, WorldPos>();
	
	const auto render_view = reg.view<SpriteAnim, WorldPos, SpriteQuad>();
	
	for (const entt::entity e : render_view) {
		SpriteAnim& s = render_view.get<SpriteAnim>(e);
		if (s.show) {
			s.frame_s += s.anim_speed*clk.dt_s;
			while (s.frame_s > 1) {
				s.sprite.animate();
				s.frame_s -= 1;
			}
			ReiDV::drawQuad( man_tex.getTexAt(s.sprite.res_id),
				man_tex.getRegionAt(s.sprite.res_id, s.sprite.getCurRegion()),
				render_view.get<WorldPos>(e).pos,
				render_view.get<SpriteQuad>(e).quad,
				s.sprite.tint);
		}
	}

	if (debug) {
		DrawGrid(1000, World::METRE);
	}

	EndMode3D();

	if (debug) renderDebug(clk, reg, man_tex);
	EndDrawing();
}

//call between BeginMode3D and EndMode3D
void ViewRenderer::renderAxes()
{
	DrawLine3D(
		(Vector3){World::METRE,World::METRE,World::METRE},
		(Vector3) {World::METRE*2,World::METRE,World::METRE},
		RED);
	DrawLine3D(
		(Vector3){World::METRE,World::METRE,World::METRE},
		(Vector3) {World::METRE,World::METRE*2,World::METRE},
		GREEN);
	DrawLine3D(
		(Vector3){World::METRE,World::METRE,World::METRE},
		(Vector3) {World::METRE,World::METRE,World::METRE*2},
		BLUE);
}

//call dis between BeginDrawing() and EndDrawing() but not in 3D
void ViewRenderer::renderAxes2D(Vector2 screen_pos, float cam_rot)
{
	Rectangle line_thick = {
		screen_pos.x,
		screen_pos.y,
		World::METRE*0.25,
		World::METRE*5
	};
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
	ManTex& man_tex)
{
	stringstream debugtxt;
	debugtxt << "fps: " << clk.fps() << "\n";
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
