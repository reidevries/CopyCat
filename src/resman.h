/*
 * resman.h
 *
 *  Created on: Mar 23, 2020
 *      Author: raeaw
 *      This is the resource manager,
 *      it loads resources (textures, sounds etc) from files,
 *      unloads resources if they haven't been used for a long time,
 *		and
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
		const int max_age = 10000; //in ms. if a texture has been offscreen longer than this, unload it
		const std::string tex_directory = "sprite/";
		bool debug;
		TexRes blank_tex, debug_tex; //if a texture isn't loaded yet, load one of these in the mean time
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
		std::unique_ptr<TexSprite> createTextureSprite(std::string tex_name, float screen_scale);
		void loadTextures(float dt);
		void ageTextures(float dt);
		void deleteUnused();
};

#endif /* SRC_RESMAN_H_ */
