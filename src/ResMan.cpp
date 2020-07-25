/*
 * resman.cpp
 *
 *  Created on: Mar 27, 2020
 *      Author: rei de vries
 *      Implementation of a resource manager
 */

#include "ResMan.h"

using namespace std;

const string ResMan::tex_directory = "sprite/";

string ResMan::constructImageFilename(string name) const
{
	return tex_directory + name + ".png";
}

string ResMan::constructAtlasFilename(string name) const
{
	return tex_directory + name + ".atlas";
}


ResMan::ResMan(const bool set_debug) : debug(set_debug)
{
	requestTex("debug");
}

ResMan::ResMan()
	: ResMan(false)
{
}

void ResMan::storeAtlas(string atlas_name, map<string,Rectangle> regions_list)
{
	int cur_id = tex_buf.find(atlas_name);
	for (auto& element : regions_list) {
		region_bufs[cur_id].push(element.first, element.second);
	}
}

void ResMan::parseAtlasString(string atlas)
{
	stringstream ss(atlas);
	string buf;
	uint8_t region_id = Res::REGION_BUF_SIZE;
	AtlasTok cur_token = AtlasTok::s;
	Rectangle cur_rectangle;
	string cur_name;
	//parse the atlas word by word
	while (ss >> buf) {
		//lines that start with "s" represent a region of the atlas
		if (buf.compare("s") == 0) {
			region_id = (region_id+1)%Res::REGION_BUF_SIZE;
			cur_token = AtlasTok::s;
			cur_rectangle = {0,0,0,0};
			cur_name = "";
		}

		switch(cur_token) {
		case (AtlasTok::nameID):
			cur_name = buf;
			//if the last character isn't a digit, append '0'
			if (!isdigit(cur_name.back())) {
				cur_name = cur_name + "0";
			}
			break;
		case (AtlasTok::positionX):
			cur_rectangle.x = stoi(buf);
			break;
		case (AtlasTok::positionY):
			cur_rectangle.y = stoi(buf);
			break;
		case (AtlasTok::sourceSizeWidth):
			cur_rectangle.width = stoi(buf);
			break;
		case (AtlasTok::sourceSizeHeight):
			cur_rectangle.height = stoi(buf);
			//this is the last value we need to parse,
			//so we can now add the rectangle to the regions list
			region_bufs[image_buf.ID].push(cur_name, cur_rectangle);
			break;
		default:
			break;
		}

		cur_token = static_cast<AtlasTok>(static_cast<int>(cur_token)+1);
	}

}

uint8_t ResMan::requestTex(string name)
{
	uint8_t atlas_id;
	if (tex_buf.has(name)) {
		atlas_id = tex_buf.find(name);
	} else {
		atlas_id = tex_buf.push(name, Texture2D());
		tex_load_queue.push(name);
	}
	//initialize associated region buf
	if (atlas_id < Res::TEX_BUF_SIZE) {
		region_bufs[atlas_id] = ResBuf<Rectangle, Res::REGION_BUF_SIZE>();
	} else {
		DebugPrinter::printDebug(0, "ResMan::requestTex",
			"no free space left for " + name + "!");
	}
	return atlas_id;
}

array<uint8_t, Res::MAX_ANIM_FRAMES> ResMan::requestRegions(
	uint8_t atlas_id,
	string name,
	uint8_t num_frames)
{
	array<uint8_t, Res::MAX_ANIM_FRAMES> accumulator;
	//don't queue regions for nonexistent atlas
	if (!tex_buf.isFree(atlas_id)) {
		if (debug && region_bufs[atlas_id].has(name)) {
			DebugPrinter::printDebug(0, "ResMan::requestRegions",
				"Oi, Rei made a boo-boo somewhere, " + name
				+ " should have a suffix, should look like "
				+ name + "0");
		}

		//if num_frames==0, nonetheless request one region called ${name}0
		if (num_frames == 0) num_frames = 1;

		for (int i = 0; i < num_frames; ++i) {
			string frame_name = name+to_string(i);
			accumulator[i]
				= region_bufs[atlas_id].findOrPush(frame_name, Rectangle());
		}
	} else {
		DebugPrinter::printDebug(0, "ResMan::queueRegions",
			"error: atlas id " + to_string(atlas_id)
			+ " should be queued before loading the region " + name);
	}
	return accumulator;
}

//request a single region without animation frames
uint8_t ResMan::requestRegion(uint8_t atlas_id, string name)
{
	if (!tex_buf.isFree(atlas_id)) {
		string frame_name = name+"0";
		return region_bufs[atlas_id].findOrPush(frame_name, Rectangle());
	} else {
		DebugPrinter::printDebug(0, "ResMan::queueRegions",
			"error: atlas id " + to_string(atlas_id)
			+ " should be queued before loading the region " + name);
		return Res::REGION_BUF_SIZE;
	}
}

void ResMan::freeTexByIndex(uint8_t index) {
	UnloadTexture(tex_buf.pop(index));
	region_bufs[index].clear();
}

uint8_t ResMan::getAtlasID(string name)
{
	if (tex_buf.has(name)) return tex_buf.find(name);
	else return 0;
}

void ResMan::loadNextImage()
{
	//skip this method if queue's empty
	if (tex_load_queue.empty()) return;

	string to_load = tex_load_queue.front();
	uint8_t cur_id = 0;
	try {
		cur_id = tex_buf.find(to_load);
	} catch (const std::out_of_range& e) {
		DebugPrinter::printDebug(0, "ResMan::loadNextImage",
			to_load + " should've been initialised, but hasn't");
		return;
	}
	DebugPrinter::printDebug(2, "ResMan::loadNextImage",
		"loading " + to_load + " into buf at index " + to_string(cur_id));

	//first, load the png into Image data on the CPU
	string image_filename = constructImageFilename(to_load);
	if (FileExists(image_filename.c_str())) {
		image_buf.name = to_load;
		image_buf.image = LoadImage(image_filename.c_str());
		image_buf.rect = {
			0,
			0,
			static_cast<float>(image_buf.image.width),
			static_cast<float>(image_buf.image.height)
		};
		image_buf.ID = cur_id;
		image_buf.atlas = "";
	} else {
		DebugPrinter::printDebug(0, "ResMan::loadNextImage",
			"error loading " + image_filename);
	}
	tex_load_queue.pop();

	//second, load the atlas into a std::string if the file exists
	//if the file doesn't exist, set the texture to be one big region
	string atlas_filename = constructAtlasFilename(to_load);
	if (FileExists(atlas_filename.c_str())) {
		image_buf.atlas = static_cast<string>(LoadFileText(
			atlas_filename.c_str()));
		DebugPrinter::printDebug(3, "ResMan::loadNextImage",
			"found atlas file at " + atlas_filename);
	} else {
		region_bufs.at(cur_id).replace(requestRegion(cur_id, image_buf.name),
			image_buf.rect);
		DebugPrinter::printDebug(2, "ResMan::loadNextImage",
			"no available atlas file, using this rect: "
			+ VectorMath::printRect(image_buf.rect));
	}
}

void ResMan::loadNextTex()
{
	//skip method if the image buf is empty
	if (image_buf.ID >= Res::TEX_BUF_SIZE) return;

	//first, load the texture from image_buf and unload the image
	tex_buf.replace(image_buf.ID, LoadTextureFromImage(image_buf.image));
	UnloadImage(image_buf.image);

	//second, if there's a texture atlas to be parsed, and the
	//region buf isn't already full, parse the atlas
	if (region_bufs[image_buf.ID].count() == 0) {
		parseAtlasString(image_buf.atlas);
	} else {
		if (image_buf.atlas.size() > 0) {
			DebugPrinter::printDebug(0, "ResMan::loadNextTex",
				"error: there's atlas data to be loaded for "
				+ image_buf.name + " but its regions_buf isn't empty ");
		}
	}

	image_buf = ImageBuf(); //reset image_buf
}

bool ResMan::isRegionLoaded(uint8_t atlas_id, uint8_t region_id) const
{
	if (isTexLoaded(atlas_id)) {
		return (!region_bufs.at(atlas_id).isFree(region_id));
	} else return false;
}

bool ResMan::isRegionLoaded(string atlas_name, string region_name) const
{
	if (isTexLoaded(atlas_name)) {
		uint8_t cur_id = tex_buf.find(atlas_name);
		return (region_bufs.at(cur_id).has(region_name));
	} else return false;
}

bool ResMan::isTexLoaded(uint8_t atlas_id) const
{
	if (!tex_buf.isFree(atlas_id)) {
		if (tex_buf.get(atlas_id).id > 0) {
			return true;
		} else {
			return false;
		}
	} else {
		DebugPrinter::printDebug(0, "ResMan::isTexLoaded",
			"error: for some reason atlas at " + to_string(atlas_id)
			+ " exists but isn't loaded");
		return false;
	}
}

bool ResMan::isTexLoaded(string atlas_name) const
{
	if (tex_buf.has(atlas_name)) {
		uint8_t cur_id = tex_buf.find(atlas_name);
		return isTexLoaded(cur_id);
	} else return false;
}

SpriteAnim ResMan::constructSprite(string atlas_name,
	string region_name,
	uint8_t num_frames,
	Vector2 size)
{
	SpriteAnim s;
	s.atlas_name = atlas_name;
	s.region_name = region_name;

	//if the atlas/regions are already loaded, queueTex and queueRegions will
	//spit out the existing ids instead of queuing them
	s.res_id = requestTex(atlas_name);
	s.region_ids = requestRegions(s.res_id, region_name,num_frames);
	if (s.region_ids.size() == 0) {
		DebugPrinter::printDebug(0, "ResMan::constructSprite",
			"BIG error, requestRegions(" + to_string(s.res_id) + ",'"
			+ region_name + "'," + to_string(num_frames)
			+ ") didn't return any region ids!");
		return s; //this return will be considered invalid cuz num_frames=0
	}

	s.num_frames = num_frames;
	return s;
}

SpriteAnim ResMan::constructSprite(string atlas_name,
	Vector2 size)
{
	return constructSprite(atlas_name, atlas_name, 0, size);
}

Rectangle ResMan::getRegionAt(uint8_t atlas_id, uint8_t region_id)
{
	if (atlas_id < Res::TEX_BUF_SIZE
		&& region_id < Res::REGION_BUF_SIZE
		&& !tex_buf.isFree(atlas_id)
		&& !region_bufs[atlas_id].isFree(region_id)) {
		return region_bufs[atlas_id].at(region_id);
	}
	DebugPrinter::printDebug(3, "ResMan::getRegionAt",
		"resource id " + to_string(atlas_id) +
		" or region id " + to_string(region_id) + " invalid");
	return Rectangle();
}

Texture2D ResMan::getTexAt(uint8_t id)
{
	if (id < Res::TEX_BUF_SIZE
		&& !tex_buf.isFree(id) ) {
		return tex_buf.get(id);
	}
	DebugPrinter::printDebug(3, "ResMan::getTexAt",
		"resource id " + to_string(id) + " invalid");
	return Texture2D();
}

ResMan::~ResMan()
{
	if (image_buf.ID < Res::TEX_BUF_SIZE) UnloadImage(image_buf.image);
	for (auto& tex : tex_buf.popAll()) UnloadTexture(tex);
	for (auto& region_buf : region_bufs) {
		region_buf.popAll();
		//don't need to unload rectangles, they're just 4 ints
	}
}
