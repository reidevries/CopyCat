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

ResMan::ResMan(bool debug)
{
	this->debug = debug;
	blank_tex = TexRes();
	debug_tex = TexRes("sprite/debug.png");
}

ResMan::ResMan()
	: ResMan(false)
{
}

shared_ptr<TexRes> ResMan::getTexture(string filename)
{
	if (tex_buf.count(filename) == 0) { //if the texture isn't loaded, schedule it for loading
		load_queue.push_back(filename);
		tex_buf.insert({
			filename,
			make_shared<TexRes>(debug ? debug_tex : blank_tex)
		});
		DebugPrinter::printDebug("ResMan::getTexture",
			filename + " was added to the buffer to be loaded", 2);
	} else {
		DebugPrinter::printDebug("ResMan::getTexture",
			filename + " was already in the buffer, serving", 2);
	}
	return tex_buf[filename];
}

TexRegion ResMan::getRegion(string filename)
{
	return TexRegion(getTexture(filename));
}

TexRegion ResMan::getRegion(string filename, Rectangle src_rect)
{
	return TexRegion(getTexture(filename), src_rect);
}

//texturesprites follow the format of tex_directory/[name][numeral].png,
//where numeral is the frame index.
//'0' numeral is a synonym of not having a numeral.
//numeral is excluded if set to a negative number
string ResMan::constructTextureFilename(string tex_name, int numeral)
{
	//if passed a negative numeral, it will not insert it into the filename
	string numeral_str =
		(numeral < 0) ? string("") : to_string(numeral);
	return tex_directory + tex_name + numeral_str + ".png";
}

string ResMan::constructTextureFilename(string tex_name)
{
	return constructTextureFilename(tex_name, -1);
}

void ResMan::loadTextures(float dt)
{
	const int number_to_load = 5;
	int i = 0;
	while (i < number_to_load && load_queue.size() > 0) {
		//replace the default texture with the newly loaded texture
		tex_buf[load_queue[0]].reset(new TexRes(load_queue[0].c_str()));
		DebugPrinter::printDebug("ResMan::loadTextures",
			"loaded texture " + load_queue[0], 2);
		load_queue.erase(load_queue.begin());
		i++;
	}
}

void ResMan::ageTextures(float dt)
{
	for (auto tex : tex_buf) {
		tex.second->age(dt);
	}
}

unique_ptr<TexSprite> ResMan::createTextureSprite(string tex_name,
	SpriteType type)
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

	return make_unique<TexSprite>(tex_name, textures, type);
}

void ResMan::deleteUnused()
{
	for (auto it = tex_buf.begin(); it != tex_buf.end();) {
		if (it->second->getAge() > max_age) {
			DebugPrinter::printDebug("ResMan::deleteUnused",
				(*it).first + " was erased", 2);
			it = tex_buf.erase(it);
		} else {
			++it;
		}
	}
}
