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

void ResMan::parseAtlasTok(const AtlasTok tok, const std::string word, 
		std::string& cur_name, Rectangle& cur_rect)
{
	switch(tok) {
	case (AtlasTok::nameID):
		cur_name = word;
		//if the last character isn't a digit, append '0' for compatibility
		if (!isdigit(cur_name.back())) {
			cur_name = cur_name + "0";
		}
		break;
	case (AtlasTok::positionX):
		try {
			cur_rect.x = stoi(word);
		} catch (const invalid_argument& e) {
			stringstream ss;
			ss << "stoi exception at token '" << word 
				<< "' for token type positionX";
			DebugPrinter::printDebug(0, "ResMan::parseAtlasTok", ss.str());
		}
		break;
	case (AtlasTok::positionY):
		try {
			cur_rect.y = stoi(word);
		} catch (const invalid_argument& e) {
			stringstream ss;
			ss << "stoi exception at token '" << word 
				<< "' for token type positionY";
			DebugPrinter::printDebug(0, "ResMan::parseAtlasTok", ss.str());
		}
		break;
	case (AtlasTok::sourceSizeWidth):
		try {
			cur_rect.width = stoi(word);
		} catch (const invalid_argument& e) {
			stringstream ss;
			ss << "stoi exception at token '" << word 
				<< "' for token type sourceSizeWidth";
			DebugPrinter::printDebug(0, "ResMan::parseAtlasTok", ss.str());
		}
		break;
	case (AtlasTok::sourceSizeHeight):
		try {
			cur_rect.height = stoi(word);
		} catch (const invalid_argument& e) {
			stringstream ss;
			ss << "stoi exception at token '" << word 
				<< "' for token type sourceSizeHeight";
			DebugPrinter::printDebug(0, "ResMan::parseAtlasTok", ss.str());
		}
	default:
		break;
	}
}

void ResMan::parseAtlasString(std::string_view atlas)
{
	//data for the region currently being parsed
	uint8_t region_id = Res::REGION_BUF_SIZE-1;
	AtlasTok cur_tok = AtlasTok::s;
	Rectangle cur_rect;
	string cur_name;
	
	string_view cur_line;
	size_t line_pos = 0;				//pos of start of current line
	size_t eol_pos = 0;		//pos of end of current line
	size_t line_length = 0;				//length of the current line
	
	string_view cur_word;
	size_t word_pos = 0;				//pos of start of current word
	size_t word_end_pos = atlas.size(); //pos of first whitespace after word
	size_t word_length = 0;				//length of current word
	
	int line_count = 0;
	
	
	while ((line_pos = atlas.find_first_not_of("\n", eol_pos)) 
		!= string_view::npos) {
		eol_pos = atlas.find('\n', line_pos);
		line_length = (eol_pos-line_pos)%atlas.size();
		cur_line = atlas.substr(line_pos, line_length);
		
		if (cur_line.front() == 's') {
			
			word_pos = 0;
			word_end_pos = 0;
			
			region_id = (region_id+1)%Res::REGION_BUF_SIZE;
			cur_tok = AtlasTok::s;
			cur_rect = {0,0,0,0};
			cur_name = "";
			while ((word_pos = cur_line.find_first_not_of(" \t", word_end_pos))
				!= string_view::npos) {
				//set the word pos to the start of the next word
				word_end_pos = cur_line.find_first_of(" \t", word_pos);
				word_length = (word_end_pos-word_pos)%cur_line.size();
				cur_word = cur_line.substr(word_pos, word_length);
			
				parseAtlasTok(cur_tok, string(cur_word), cur_name, cur_rect);
			
				if (cur_tok == AtlasTok::sourceSizeHeight) {
					//this is the last value we need to parse,
					//so we can now add the rectangle to the regions list
					//and move on to the next line (if there is a next line)
					region_bufs[image_buf.ID].pushOrReplace(cur_name, cur_rect);
					
					stringstream ss;
					ss << "parsed region " << cur_name 
						<< " to rectangle " 
						<< VectorMath::printRect(cur_rect);
					DebugPrinter::printDebug(5, "ResMan::parseAtlasString",
						ss.str());
					break;
				}
			
				cur_tok = static_cast<AtlasTok>(static_cast<int>(cur_tok)+1);
			}
		}
		
		++line_count;
	}
	
	stringstream ss;
	ss << "final buffer state: ";
	for (auto const& item : region_bufs[image_buf.ID].getNames()) {
		if (item != ResBuf<Rectangle, Res::REGION_BUF_SIZE>::free_entry_name) {
			ss << item << ", ";
		}
	}
	DebugPrinter::printDebug(4, "ResMan::parseAtlasString", ss.str());
}

uint8_t ResMan::requestTex(const string& name)
{
	uint8_t atlas_id = Res::TEX_BUF_SIZE;
	if (tex_buf.has(name)) {
		atlas_id = tex_buf.find(name);
	} else {
		atlas_id = tex_buf.push(name, Texture2D());
		//initialize associated region buf
		if (atlas_id < Res::TEX_BUF_SIZE) {
			region_bufs[atlas_id] = ResBuf<Rectangle, Res::REGION_BUF_SIZE>();
		} else {
			DebugPrinter::printDebug(0, "ResMan::requestTex",
				"no free space left for " + name + "!");
		}
		tex_load_queue.push(name);
	}
	return atlas_id;
}

array<uint8_t, Res::MAX_ANIM_FRAMES> ResMan::requestRegions(
	uint8_t atlas_id,
	const string& name,
	uint8_t num_frames)
{
	array<uint8_t, Res::MAX_ANIM_FRAMES> accumulator;
	//don't queue regions for nonexistent atlas
	if (!tex_buf.isFree(atlas_id)) {
		//if num_frames==0, nonetheless request one region called ${name}0
		if (num_frames == 0) num_frames = 1;

		for (int i = 0; i < num_frames; ++i) {
			if (isdigit(name.back())) {
				accumulator[i]
					= region_bufs[atlas_id].findOrPush(name, Rectangle());
			} else {
				string frame_name = name+to_string(i);
				accumulator[i]
					= region_bufs[atlas_id].findOrPush(frame_name, Rectangle());
			}
		}
	} else {
		DebugPrinter::printDebug(0, "ResMan::queueRegions",
			"error: atlas id " + to_string(atlas_id)
			+ " should be queued before loading the region " + name);
	}
	return accumulator;
}

//request a single region without animation frames
uint8_t ResMan::requestRegion(uint8_t atlas_id, const string& name)
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

uint8_t ResMan::getAtlasID(const string& name) const
{
	if (tex_buf.has(name)) {
		return tex_buf.find(name);
	} else return Res::TEX_BUF_SIZE;
}

uint8_t ResMan::getRegionID(const string& atlas_name, 
							const string& region_name) const
{
	if (tex_buf.has(atlas_name)) {
		const ResBuf<Rectangle, Res::REGION_BUF_SIZE>& region_buf =  
			region_bufs[tex_buf.find(atlas_name)];
		if (region_buf.has(region_name)) return region_buf.find(region_name);
	} 
	return Res::REGION_BUF_SIZE;
}

uint8_t ResMan::getRegionID(uint8_t atlas_id, 
							const string& region_name) const
{
	if (atlas_id < Res::TEX_BUF_SIZE) {
		const ResBuf<Rectangle, Res::REGION_BUF_SIZE>& region_buf =
			region_bufs[atlas_id];
		if (region_buf.has(region_name)) return region_buf.find(region_name);
	}
	return Res::REGION_BUF_SIZE;
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
			"no available atlas file for " + atlas_filename
			+ " using this rect: "
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

	try {
		parseAtlasString(image_buf.atlas);
	} catch (const range_error& e) {
		stringstream ss;
		ss << "error parsing atlas string " << image_buf.name 
			<< ", ResBuf threw range_error: " << e.what()
			<< endl << "Most likely, the atlas contains too many regions";
		DebugPrinter::printDebug(0, "ResMan::loadNextTex", ss.str());
	}
	image_buf = ImageBuf(); //reset image_buf
}

bool ResMan::isRegionLoaded(uint8_t atlas_id, uint8_t region_id) const
{
	if (isTexLoaded(atlas_id)) {
		return (!region_bufs.at(atlas_id).isFree(region_id));
	} else return false;
}

bool ResMan::isRegionLoaded(const string& atlas_name, 
							const string& region_name) const
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

bool ResMan::isTexLoaded(const string& atlas_name) const
{
	if (tex_buf.has(atlas_name)) {
		uint8_t cur_id = tex_buf.find(atlas_name);
		return isTexLoaded(cur_id);
	} else return false;
}

SpriteAnim ResMan::constructSprite(const string& atlas_name,
	const string& region_name,
	uint8_t num_frames,
	Vector2 size)
{
	SpriteAnim s;
	s.atlas_name = atlas_name;
	s.region_name = region_name;

	//if the atlas/regions are already loaded, queueTex and queueRegions will
	//spit out the existing ids instead of queuing them
	s.res_id = requestTex(atlas_name);
	s.region_ids = requestRegions(s.res_id, region_name, num_frames);
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

SpriteAnim ResMan::constructSprite(const string& atlas_name,
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

Rectangle ResMan::getRegionAt(uint8_t atlas_id, const std::string& region_name)
{
	uint8_t region_id = getRegionID(atlas_id, region_name);
	return getRegionAt(atlas_id, region_id);
}


Rectangle ResMan::getRegionAt(const std::string& atlas_name, 
							  const std::string& region_name)
{
	uint8_t atlas_id = getAtlasID(atlas_name);
	return getRegionAt(atlas_id, region_name);
}

vector<string> ResMan::getRegionNames(
	const string& atlas_name) const
{
	vector<string> accumulator;
	uint8_t id = getAtlasID(atlas_name);
	if (id >= Res::TEX_BUF_SIZE) return vector<string>();
	
	auto const& names = region_bufs.at(static_cast<int>(id));
	for (auto const& name : names.getNames()) {
		accumulator.push_back(name);
	}
	
	return accumulator;
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
