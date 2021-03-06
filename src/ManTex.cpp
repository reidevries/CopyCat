/* Created on: Mar 27, 2020
 *      Author: rei de vries
 */

#include "ManTex.hpp"

using namespace std;

const string ManTex::tex_directory = "sprite/";

string ManTex::constructImageFilename(const string& name) const 
{
	return tex_directory + name + ".png";
}

string ManTex::constructAtlasFilename(const string& name) const
{
	return tex_directory + name + ".atlas";
}

string ManTex::constructRegionName(string_view name, size_t number) const
{
	stringstream ss;
	if (number > Res::REGION_BUF_SIZE) {
		ss << number << " is greater than region buf size which is " 
			<< Res::REGION_BUF_SIZE << endl; 
		throw out_of_range(ss.str());
	}
	ss << name << setfill('0') << setw(Res::MAX_ANIM_BITS) << number;
	return ss.str();
}


ManTex::ManTex(const bool set_debug) : debug(set_debug)
{
	requestTex("debug");
}

ManTex::ManTex()
	: ManTex(false)
{
}

void ManTex::storeAtlas(string atlas_name, map<string,Rectangle> regions_list)
{
	int cur_id = tex_buf.find(atlas_name);
	for (auto const& [name, region] : regions_list) {
		region_bufs[cur_id].push(name, region);
	}
}

void ManTex::parseAtlasTok(const AtlasTok tok, const std::string word, 
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
			DebugPrinter::printDebug(0, "ManTex::parseAtlasTok", ss.str());
		}
		break;
	case (AtlasTok::positionY):
		try {
			cur_rect.y = stoi(word);
		} catch (const invalid_argument& e) {
			stringstream ss;
			ss << "stoi exception at token '" << word 
				<< "' for token type positionY";
			DebugPrinter::printDebug(0, "ManTex::parseAtlasTok", ss.str());
		}
		break;
	case (AtlasTok::sourceSizeWidth):
		try {
			cur_rect.width = stoi(word);
		} catch (const invalid_argument& e) {
			stringstream ss;
			ss << "stoi exception at token '" << word 
				<< "' for token type sourceSizeWidth";
			DebugPrinter::printDebug(0, "ManTex::parseAtlasTok", ss.str());
		}
		break;
	case (AtlasTok::sourceSizeHeight):
		try {
			cur_rect.height = stoi(word);
		} catch (const invalid_argument& e) {
			stringstream ss;
			ss << "stoi exception at token '" << word 
				<< "' for token type sourceSizeHeight";
			DebugPrinter::printDebug(0, "ManTex::parseAtlasTok", ss.str());
		}
	default:
		break;
	}
}

void ManTex::parseAtlasString(std::string_view atlas)
{
	//data for the region currently being parsed
	size_t region_id = Res::REGION_BUF_SIZE-1;
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
					stringstream ss;
					//this is the last value we need to parse,
					//so we can now add the rectangle to the regions list
					//and move on to the next line (if there is a next line)
					try {
						region_bufs[image_buf.ID].replace(
							region_bufs[image_buf.ID].find(cur_name), 
							cur_rect);
					} catch (const out_of_range& e) {
						ss << "region by name of " << cur_name 
							<< " not found in region_bufs[" 
							<< image_buf.ID 
							<< "] for image name " << image_buf.name << endl;
						DebugPrinter::printDebug(0, 
							"ManTex::parseAtlasString",
							ss.str());
					}
					
					ss.str("");
					ss << "parsed region " << cur_name 
						<< " to rectangle " 
						<< VectorMath::printRect(cur_rect);
					DebugPrinter::printDebug(4, 
						"ManTex::parseAtlasString",
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
	DebugPrinter::printDebug(4, "ManTex::parseAtlasString", ss.str());
}

size_t ManTex::requestTex(const string& name)
{
	size_t atlas_id = Res::TEX_BUF_SIZE;
	if (tex_buf.has(name)) {
		atlas_id = tex_buf.find(name);
	} else {
		atlas_id = tex_buf.push(name, Texture2D());
		//initialize associated region buf
		if (atlas_id < Res::TEX_BUF_SIZE) {
			region_bufs[atlas_id] = ResBuf<Rectangle, Res::REGION_BUF_SIZE>();
		} else {
			DebugPrinter::printDebug(0, "ManTex::requestTex",
				"no free space left for " + name + "!");
		}
		tex_load_queue.push(name);
	}
	return atlas_id;
}

array<size_t, Res::MAX_ANIM_FRAMES> ManTex::requestRegions(
	size_t atlas_id,
	const string& name,
	size_t num_frames)
{
	if (num_frames > Res::MAX_ANIM_FRAMES) {
		stringstream ss;
		ss << "num_frames = " << num_frames 
			<< " which is greater than " << Res::MAX_ANIM_FRAMES
			<< " so the number of frames is being capped" << endl;
		DebugPrinter::printDebug(0, "ManTex::requestRegions", ss.str());
		num_frames = Res::MAX_ANIM_FRAMES;
	}
	array<size_t, Res::MAX_ANIM_FRAMES> accumulator;
	
	//remove digits from end of name
	string_view real_name(name);
	auto trim_index = real_name.find_first_of("0123456789");
	if (trim_index != string_view::npos) {
		real_name.remove_suffix(real_name.size()-trim_index);
		DebugPrinter::printDebug(0, 
			"ManTex::requestRegions", 
			string(real_name));
	}
	
	//don't queue regions for nonexistent atlas
	if (!tex_buf.isFree(atlas_id)) {
		//if num_frames==0, nonetheless request one region called ${name}0
		if (num_frames == 0) {
			accumulator[0] = region_bufs[atlas_id].findOrPush(
				constructRegionName(real_name, 0), 
				Rectangle());
		}

		for (int i = 0; i < num_frames; ++i) {
			accumulator[i] = region_bufs[atlas_id].findOrPush(
				constructRegionName(real_name, i),
				Rectangle());
		}
	} else {
		DebugPrinter::printDebug(0, "ManTex::requestRegions",
			"error: atlas id " + to_string(atlas_id)
			+ " should be queued before loading the region " + name);
	}
	return accumulator;
}

//request a single region without animation frames
size_t ManTex::requestRegion(size_t atlas_id, const string& name)
{
	if (!tex_buf.isFree(atlas_id)) {
		return region_bufs[atlas_id].findOrPush(
			constructRegionName(string_view(name), 0), 
			Rectangle());
	} else {
		DebugPrinter::printDebug(0, "ManTex::queueRegions",
			"error: atlas id " + to_string(atlas_id)
			+ " should be queued before loading the region " + name);
		return Res::REGION_BUF_SIZE;
	}
}

void ManTex::freeTexByIndex(size_t index) 
{
	UnloadTexture(tex_buf.pop(index));
	region_bufs[index].clear();
}

size_t ManTex::getAtlasID(const string& name) const
{
	if (tex_buf.has(name)) {
		return tex_buf.find(name);
	} else return Res::TEX_BUF_SIZE;
}

size_t ManTex::getRegionID(const string& atlas_name, 
							const string& region_name) const
{
	if (tex_buf.has(atlas_name)) {
		const ResBuf<Rectangle, Res::REGION_BUF_SIZE>& region_buf =  
			region_bufs[tex_buf.find(atlas_name)];
		if (region_buf.has(region_name)) return region_buf.find(region_name);
	} 
	return Res::REGION_BUF_SIZE;
}

size_t ManTex::getRegionID(size_t atlas_id, 
							const string& region_name) const
{
	if (atlas_id < Res::TEX_BUF_SIZE) {
		const ResBuf<Rectangle, Res::REGION_BUF_SIZE>& region_buf =
			region_bufs[atlas_id];
		if (region_buf.has(region_name)) return region_buf.find(region_name);
	}
	return Res::REGION_BUF_SIZE;
}

void ManTex::loadNextImage()
{
	//skip this method if queue's empty
	if (tex_load_queue.empty()) return;

	string to_load = tex_load_queue.front();
	size_t cur_id = 0;
	try {
		cur_id = tex_buf.find(to_load);
	} catch (const std::out_of_range& e) {
		DebugPrinter::printDebug(0, "ManTex::loadNextImage",
			to_load + " should've been initialised, but hasn't");
		return;
	}
	DebugPrinter::printDebug(2, "ManTex::loadNextImage",
		"loading " + to_load + " into buf at index " + to_string(cur_id));

	//first, load the png into Image data on the CPU
	string image_filename = constructImageFilename(to_load);
	if (FileExists(image_filename.c_str())) {
		// if there is still an image in the buf somehow, unload it first
		if (image_buf.ID < Res::TEX_BUF_SIZE) UnloadImage(image_buf.image);
	 
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
		DebugPrinter::printDebug(0, "ManTex::loadNextImage",
			"error loading " + image_filename);
	}
	tex_load_queue.pop();

	//second, load the atlas into a std::string if the file exists
	//if the file doesn't exist, set the texture to be one big region
	string atlas_filename = constructAtlasFilename(to_load);
	if (FileExists(atlas_filename.c_str())) {
		image_buf.atlas = static_cast<string>(LoadFileText(
			atlas_filename.c_str()));
		DebugPrinter::printDebug(3, "ManTex::loadNextImage",
			"found atlas file at " + atlas_filename);
	} else {
		region_bufs.at(cur_id).replace(requestRegion(cur_id, image_buf.name),
			image_buf.rect);
		DebugPrinter::printDebug(2, "ManTex::loadNextImage",
			"no available atlas file for " + atlas_filename
			+ " using this rect: "
			+ VectorMath::printRect(image_buf.rect));
	}
}

void ManTex::loadNextTex()
{
	//skip method if the image buf is empty
	if (image_buf.ID >= Res::TEX_BUF_SIZE) return;

	//first, load the texture from image_buf and unload the image
	try {
		tex_buf.replace(image_buf.ID, LoadTextureFromImage(image_buf.image));
	} catch (const out_of_range& e) {
		stringstream ss;
		ss << "error replacing tex at " << image_buf.ID
			<< " with atlas named " << image_buf.name
			<< " ResBuf threw out_of_range: " << e.what() << endl;
	}
	UnloadImage(image_buf.image);

	try {
		parseAtlasString(image_buf.atlas);
	} catch (const range_error& e) {
		stringstream ss;
		ss << "error parsing atlas string " << image_buf.name 
			<< ", ResBuf threw range_error: " << e.what()
			<< endl << "Most likely, the atlas contains too many regions";
		DebugPrinter::printDebug(0, "ManTex::loadNextTex", ss.str());
	}
	image_buf = ImageBuf(); //reset image_buf
}

bool ManTex::isRegionLoaded(size_t atlas_id, size_t region_id) const
{
	if (isTexLoaded(atlas_id)) {
		return (!region_bufs.at(atlas_id).isFree(region_id));
	} else return false;
}

bool ManTex::isRegionLoaded(const string& atlas_name, 
							const string& region_name) const
{
	if (isTexLoaded(atlas_name)) {
		size_t cur_id = tex_buf.find(atlas_name);
		return (region_bufs.at(cur_id).has(region_name));
	} else return false;
}

bool ManTex::isTexLoaded(size_t atlas_id) const
{
	if (!tex_buf.isFree(atlas_id)) {
		if (tex_buf.get(atlas_id).id > 0) {
			return true;
		} else {
			return false;
		}
	} else {
		DebugPrinter::printDebug(0, "ManTex::isTexLoaded",
			"error: for some reason atlas at " + to_string(atlas_id)
			+ " exists but isn't loaded");
		return false;
	}
}

bool ManTex::isTexLoaded(const string& atlas_name) const
{
	if (tex_buf.has(atlas_name)) {
		size_t cur_id = tex_buf.find(atlas_name);
		return isTexLoaded(cur_id);
	} else return false;
}

ResSprite ManTex::constructSprite(const string& atlas_name,
	const string& region_name,
	size_t num_frames,
	Vector2 size)
{
	ResSprite s;
	s.atlas_name = atlas_name;
	s.region_name = region_name;

	//if the atlas/regions are already loaded, queueTex and queueRegions will
	//spit out the existing ids instead of queuing them
	s.res_id = requestTex(atlas_name);
	s.region_ids = requestRegions(s.res_id, region_name, num_frames);

	s.num_frames = num_frames;
	return s;
}

ResSprite ManTex::constructSprite(const string& atlas_name,
	Vector2 size)
{
	return constructSprite(atlas_name, atlas_name, 0, size);
}

Rectangle ManTex::getRegionAt(size_t atlas_id, size_t region_id)
{
	if (atlas_id < Res::TEX_BUF_SIZE
		&& region_id < Res::REGION_BUF_SIZE
		&& !tex_buf.isFree(atlas_id)
		&& !region_bufs[atlas_id].isFree(region_id)) {
		return region_bufs[atlas_id].at(region_id);
	}
	DebugPrinter::printDebug(3, "ManTex::getRegionAt",
		"resource id " + to_string(atlas_id) +
		" or region id " + to_string(region_id) + " invalid");
	return Rectangle();
}

Rectangle ManTex::getRegionAt(size_t atlas_id, const std::string& region_name)
{
	size_t region_id = getRegionID(atlas_id, region_name);
	return getRegionAt(atlas_id, region_id);
}


Rectangle ManTex::getRegionAt(const std::string& atlas_name, 
							  const std::string& region_name)
{
	size_t atlas_id = getAtlasID(atlas_name);
	return getRegionAt(atlas_id, region_name);
}

vector<string> ManTex::getRegionNames(
	const string& atlas_name) const
{
	vector<string> accumulator;
	size_t id = getAtlasID(atlas_name);
	if (id >= Res::TEX_BUF_SIZE) return vector<string>();
	
	auto const& names = region_bufs.at(static_cast<int>(id));
	for (auto const& name : names.getNames()) {
		accumulator.push_back(name);
	}
	
	return accumulator;
}

Texture2D ManTex::getTexAt(size_t id)
{
	if (id < Res::TEX_BUF_SIZE
		&& !tex_buf.isFree(id) ) {
		return tex_buf.get(id);
	}
	DebugPrinter::printDebug(3, "ManTex::getTexAt",
		"resource id " + to_string(id) + " invalid");
	return Texture2D();
}

ManTex::~ManTex()
{
	if (image_buf.ID < Res::TEX_BUF_SIZE) UnloadImage(image_buf.image);
	for (auto& tex : tex_buf.popAll()) UnloadTexture(tex);
	for (auto& region_buf : region_bufs) {
		region_buf.popAll();
		//don't need to unload rectangles, they're just 4 ints
	}
}
