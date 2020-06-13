/*
 * resman.h
 *
 *  Created on: Mar 23, 2020
 *      Author: rei de vries
 *      This is the resource manager,
 *      it loads resources (textures, sounds etc) from files,
 *      unloads resources if they haven't been used for a long time
 */

#ifndef SRC_RESMAN_H_
#define SRC_RESMAN_H_

#include <raylib.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include "debugprinter.h"
#include "texres.h"
#include "texsprite.h"

class ResMan {
private:
	//if a texture has been offscreen longer than this, unload it
	const int max_age = 10000; //in ms
	const std::string tex_directory = "sprite/";
	bool debug;
	TexRes blank_tex, debug_tex; //default textures
	std::vector<std::string> load_queue;
	std::unordered_map<std::string, std::shared_ptr<TexRes>> tex_buf;

	std::string constructTextureFilename(std::string tex_name, int numeral);
	std::string constructTextureFilename(std::string tex_name);
public:
	ResMan(bool debug);
	ResMan();
	std::shared_ptr<TexRes> getTexture(std::string filename);
	TexRegion getRegion(std::string filename);
	TexRegion getRegion(std::string filename, Rectangle src_rect);
	std::unique_ptr<TexSprite> createTextureSprite(std::string tex_name, SpriteType type);
	void loadTextures(float dt);
	void ageTextures(float dt);
	void deleteUnused();
};

#endif /* SRC_RESMAN_H_ */
