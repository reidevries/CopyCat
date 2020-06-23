/*
 * resman.cpp
 *
 *  Created on: Mar 27, 2020
 *      Author: rei de vries
 *      Implementation of a resource manager
 */

#include "resman.h"

#include "texres.h"

using namespace std;

ResMan::ResMan(const bool set_debug) : debug(set_debug)
{
	blank_tex = TexRes();
	debug_tex = TexRes("sprite/debug.png");
}

ResMan::ResMan()
	: ResMan(false)
{
}

//texturesprites follow the format of tex_directory/[name][numeral].png,
//where numeral is the frame index.
//'0' numeral is a synonym of not having a numeral.
//numeral is excluded if set to a negative number
string ResMan::constructTextureFilename(const string tex_name,
	const int numeral) const
{
	//if passed a negative numeral, it will not insert it into the filename
	string numeral_str =
		(numeral < 0) ? string("") : to_string(numeral);
	return tex_directory + tex_name + numeral_str + ".png";
}

string ResMan::constructTextureFilename(const string tex_name) const
{
	return constructTextureFilename(tex_name, -1);
}

shared_ptr<TexRes> ResMan::getTexture(const string filename)
{
	//if the texture isn't loaded, schedule it for loading
	if (tex_buf.count(filename) == 0) {
		load_queue.push_back(filename);
		tex_buf.insert({
			filename,
			make_shared<TexRes>(debug ? debug_tex : blank_tex) //TODO make it so this doesn't have to copy the underlying texture, possibly refactoring texres in the process
		});
		DebugPrinter::printDebug("ResMan::getTexture",
			filename + " was added to the buffer to be loaded", 2);
	} else {
		DebugPrinter::printDebug("ResMan::getTexture",
			filename + " was already in the buffer, serving", 2);
	}
	return tex_buf[filename];
}

TexRegion ResMan::getRegion(const string filename)
{
	return TexRegion(getTexture(filename));
}

TexRegion ResMan::getRegion(const string filename, const Rectangle src_rect)
{
	return TexRegion(getTexture(filename), src_rect);
}

void ResMan::loadTextures()
{
	if (image_queue.size() > 0) {
		//replace the default texture with the newly loaded image
		tex_buf[image_queue[0].first]->reloadTexture(image_queue[0].second);

		DebugPrinter::printDebug("ResMan::loadTextures",
					"loaded texture " + image_queue[0].first, 2);

		UnloadImage(image_queue[0].second);
		image_queue.erase(image_queue.begin());
	}
}

void ResMan::loadImages() {
	if (load_queue.size() > 0) {
		Image loaded_image;
		loaded_image = LoadImage(load_queue[0].c_str());
		image_queue.emplace_back(
			load_queue[0],
			loaded_image
		);
		load_queue.erase(load_queue.begin());
	}
}

void ResMan::ageTextures(CatClock& clk)
{
	if (clk.tock_s) {
		for (auto tex : tex_buf) {
			tex.second->age();
		}
	}
}

unique_ptr<TexSprite> ResMan::createTextureSprite(const string tex_name,
	const SpriteType type,
	const Vector2 size)
{
	vector<TexRegion> textures;
	int i = 0;
	string filename = constructTextureFilename(tex_name);
	//initiate loading the textures for existing files
	if (!FileExists(filename.c_str())) {
		filename = constructTextureFilename(tex_name, i);
	} while (FileExists(filename.c_str())) {
		textures.push_back(getRegion(filename));
		filename = constructTextureFilename(tex_name, ++i);
	}

	if (textures.size() == 0) {
		DebugPrinter::printDebug("ResMan::createTextureSprite",
			filename + " doesn't exist!", 0);
	}

	return make_unique<TexSprite>(tex_name, type, textures, size);
}

void ResMan::deleteUnused()
{
	for (auto it = tex_buf.begin(); it != tex_buf.end();) {
		if (it->second->getAge() > max_age_ms) {
			DebugPrinter::printDebug("ResMan::deleteUnused",
				(*it).first + " was erased", 2);
			it = tex_buf.erase(it);
		} else {
			++it;
		}
	}
}
