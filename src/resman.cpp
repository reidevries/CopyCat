/*
 * resman.cpp
 *
 *  Created on: Mar 27, 2020
 *      Author: rei de vries
 *      Implementation of a resource manager
 */

#include "resman.h"

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
	newTex("debug");
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
	uint8_t region_id = Res::MAX_BUF_SIZE;
	AtlasTok cur_token = s;
	Rectangle cur_rectangle;
	string cur_name;
	//parse the atlas word by word
	while (ss >> buf) {
		//lines that start with "s" represent a region of the atlas
		if (buf.compare("s") == 0) {
			region_id = (region_id+1)%Res::MAX_BUF_SIZE;
			cur_token = s;
			cur_rectangle = {0,0,0,0};
			cur_name = "";
		}

		switch(cur_token) {
		case (nameID):
			cur_name = nameID;
			break;
		case (positionX):
			cur_rectangle.x = stoi(buf);
			break;
		case (positionY):
			cur_rectangle.y = stoi(buf);
			break;
		case (sourceSizeWidth):
			cur_rectangle.width = stoi(buf);
			break;
		case (sourceSizeHeight):
			cur_rectangle.height = stoi(buf);
			//this is the last value we need to parse,
			//so we can now add the rectangle to the regions list
			region_bufs[image_buf.ID].push(cur_name, cur_rectangle);
			break;
		default:
			break;
		}

		cur_token = static_cast<AtlasTok>(cur_token+1);
	}

}

uint8_t ResMan::newTex(string name)
{
	if (tex_buf.has(name)) return tex_buf.find(name);
	else {
		tex_load_queue.push(name);
		return tex_buf.push(name, Texture2D());
	}
}

void ResMan::freeTexByIndex(uint8_t index) {
	UnloadTexture(tex_buf.pop(index));
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
			to_load + " doesn't exist in tex_buf??");
		return;
	}

	//first, load the png into Image data on the CPU
	string image_filename = constructImageFilename(to_load);
	if (FileExists(image_filename.c_str())) {
		image_buf.name = to_load;
		image_buf.image = LoadImage(image_filename.c_str());
		image_buf.ID = cur_id;
		image_buf.atlas = "";
		tex_load_queue.pop();
	} else {
		DebugPrinter::printDebug(0, "ResMan::loadNextImage",
			"error loading " + image_filename);
	}

	//if the regions buf is already filled, skip
	if (region_bufs.at(cur_id).count() > 0) return;

	//second, load the atlas into a std::string if the file exists
	//if the file doesn't exist, set the texture to be one big region
	string atlas_filename = constructAtlasFilename(to_load);
	if (FileExists(atlas_filename.c_str())) {
		image_buf.atlas = static_cast<string>(LoadFileText(
			atlas_filename.c_str()));
	} else {
		region_bufs.at(cur_id).push(to_load, (Rectangle){
			0,
			0,
			static_cast<float>(image_buf.image.width),
			static_cast<float>(image_buf.image.height)
		});
	}
}

void ResMan::loadNextTex()
{
	//skip method if the image buf is empty
	if (image_buf.ID >= Res::MAX_BUF_SIZE) return;

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

bool ResMan::isTexLoaded(string atlas_name) const
{
	if (tex_buf.has(atlas_name)) {
		uint8_t cur_id = tex_buf.find(atlas_name);
		if (!tex_buf.isFree(cur_id)) {
			if (tex_buf.get(cur_id).id > 0) {
				return true;
			} else {
				return false;
			}
		} else {
			DebugPrinter::printDebug(0, "ResMan::isTexLoaded",
				"error: for some reason atlas " + atlas_name +
				" exists but isn't loaded");
			return false;
		}
	} else return false;
}

bool ResMan::isRegionLoaded(string atlas_name, string region_name) const
{
	if (isTexLoaded(atlas_name)) {
		uint8_t cur_id = tex_buf.find(atlas_name);
		return (region_bufs.at(cur_id).has(region_name));
	} else return false;
}

bool ResMan::requestRegion(string atlas_name, string region_name)
{
	if (isRegionLoaded(atlas_name, region_name)) {
		return true;
	} else {
		if (isTexLoaded(atlas_name)) {
			DebugPrinter::printDebug(0, "ResMan::loadRegion",
				"error: trying to access region '" + region_name +
				"' but it doesn't exist for " + atlas_name);
		} else {
			newTex(atlas_name);
		}
	}
	return false;
}

vector<uint8_t> ResMan::getRegions(string atlas_name,
	string region_name) const
{
	uint8_t cur_id = tex_buf.find(atlas_name);
	vector<uint8_t> accumulator;
	if (region_bufs.at(cur_id).count() == 0) {
		DebugPrinter::printDebug(0, "ResMan::getRegions",
			"error: nothing in region buffer for '" + atlas_name + "'");
		return accumulator;
	}

	//gets all regions containing region_name
	//regions can have a suffix '#n' where n is an integer
	//example: 'run_anim#0' 'run_anim#1' 'run_anim#2' etc
	//otherwise just the region that matches region_name will be used
	for (auto const& region : region_bufs.at(cur_id).getBufMap()) {
		if (region.first.find(region_name) != string::npos) {
			size_t suffix_pos = region.first.find_first_of("#");
			//if the region name has no suffix, or has a '#' with no numeral
			//after it, then push just the one region.
			if (suffix_pos == string::npos
				|| suffix_pos+1 >= region.first.size()) {
				accumulator.push_back(region.second);
				break;
			//otherwise, push the regions in the order of the suffices
			} else {
				string number = region.first.substr(suffix_pos+1, 1);
				uint8_t region_order = stoi(number);
				//if an error occurs and region_order is large,
				//the vector will take up a lot of memory
				//limit the number of regions to prevent this
				if (region_order < Res::MAX_BUF_SIZE) {
					accumulator.insert(accumulator.begin()+region_order,
						region.second);
				}
			}
		}
	}

	return accumulator;
}

TexSprite ResMan::constructSprite(string atlas_name,
	string region_name,
	TexSprite::Type type,
	Vector2 size)
{
	if (tex_buf.has(atlas_name)) {
		uint8_t cur_id = tex_buf.find(atlas_name);
		if (region_bufs.at(cur_id).has(region_name)) {
			return TexSprite(
				cur_id,
				region_name,
				type,
				getRegions(atlas_name, region_name),
				size
			);
		} else {
			DebugPrinter::printDebug(0, "ResMan::constructSprite",
				atlas_name + " is loaded, but " + region_name
				+ " not found in appropriate buf, panic!!!!");
			return TexSprite(cur_id, atlas_name, size);
		}
	//if the atlas isn't found, try queuing it to be loaded
	} else {
		uint8_t cur_id = newTex(atlas_name);
		return TexSprite(cur_id, atlas_name, size);
	}
}

TexSprite ResMan::constructSprite(string atlas_name,
	TexSprite::Type type,
	Vector2 size)
{
	return constructSprite(atlas_name, atlas_name, type, size);
}

Texture2D ResMan::getTexAt(uint8_t id)
{
	if (id < Res::MAX_BUF_SIZE
		&& !tex_buf.isFree(id) ) {
		return tex_buf.get(id);
	}
	DebugPrinter::printDebug(3, "ResMan::getTexAt",
		"resource id " + to_string(id) + " invalid");
	return Texture2D();
}

Rectangle ResMan::getRegionAt(uint8_t atlas_id, uint8_t region_id)
{
	if (atlas_id < Res::MAX_BUF_SIZE && region_id < Res::MAX_BUF_SIZE) {
		if (!region_bufs[atlas_id].isFree(region_id)) {
			return region_bufs[atlas_id].get(region_id);
		} else {
			DebugPrinter::printDebug(3, "ResMan::getRegionAt",
				"region at " + to_string(atlas_id) +
				"," + to_string(region_id) +
				" isn't loaded");
		}
	} else {
		DebugPrinter::printDebug(3, "ResMan::getRegionAt",
			"invalid region id " + to_string(atlas_id) +
			"," + to_string(region_id));
	}
	return Rectangle();
}

ResMan::~ResMan()
{
	if (image_buf.ID < Res::MAX_BUF_SIZE) UnloadImage(image_buf.image);
	for (auto& tex : tex_buf.popAll()) UnloadTexture(tex);
	for (auto& region_buf : region_bufs) {
		region_buf.popAll();
		//don't need to unload rectangles, they're just 4 ints
	}
}
