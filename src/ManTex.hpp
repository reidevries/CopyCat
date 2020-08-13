/*
 * ManTex.hpp
 *
 *  Created on: Mar 23, 2020
 *      Author: rei de vries
 *      This creates sprites, automatically loads texture atlases and regions
 * 		based on a filename requested by the user. If the file has already been
 * 		loaded, it gives the user its resource ID, otherwise, it queues it to
 * 		be loaded and gives the user the resource ID to use when it is loaded.
 * 		User can also request resources in advance
 */

#ifndef SRC_RESMAN_H_
#define SRC_RESMAN_H_

#include <vector>
#include <queue>
#include <map>
#include <string>
#include <string_view>	//C++17
#include <memory>
#include <sstream>
#include <cstdint>

#include "CatConf.hpp"
#include "CatClock.hpp"
#include "DebugPrinter.hpp"
#include "ResBuf.hpp"
#include "ResConstants.hpp"
#include "VectorMath.hpp"
#include "components/SpriteAnim.hpp"

class ManTex 
{
private:
	static const std::string tex_directory;

	std::string constructAtlasFilename(const std::string&) const;
	std::string constructImageFilename(const std::string&) const;
	std::string constructRegionName(std::string_view, std::size_t) const;

	const bool debug;

	std::queue<std::string> tex_load_queue;
	//buffer for storing image partway through loading
	struct ImageBuf {
		//if ID >= Res::TEX_BUF_SIZE, buf is empty
		std::size_t ID = Res::TEX_BUF_SIZE;
		std::string name;
		Image image;
		Rectangle rect;
		std::string atlas;
	};
	ImageBuf image_buf;
	//used for parsing atlas file, based on .rtpa format
	enum class AtlasTok {s,	nameID,
		originX, 			originY,
		positionX, 			positionY,
		sourceSizeWidth, 	sourceSizeHeight,
		padding,			trimmed,
		trimRecX,			trimRecY,
		trimRecWidth,		trimRecHeight
	};

	ResBuf<Texture2D, Res::TEX_BUF_SIZE> tex_buf;
	std::array<
		ResBuf<Rectangle, Res::REGION_BUF_SIZE>,
		Res::TEX_BUF_SIZE
	> region_bufs;

	//store a map of string to Rectangle in the region buf
	void storeAtlas(std::string atlas_name, std::map<std::string, Rectangle>);
	void parseAtlasTok( const AtlasTok tok, const std::string word, 
		std::string& cur_name, Rectangle& cur_rect);
	void parseAtlasString(std::string_view atlas);

	void freeTexByIndex(std::size_t index);
	std::array<std::size_t, Res::MAX_ANIM_FRAMES> requestRegions(
		std::size_t atlas_id,
		const std::string& name, 
		std::size_t num_frames);
	std::size_t requestRegion(std::size_t atlas_id, const std::string& name);
	

public:
	ManTex(const bool set_debug);
	ManTex();
	
	std::size_t requestTex(const std::string& name);
	
	// get IDs in const-fashion without requesting them
	std::size_t getAtlasID(const std::string& name) const;
	std::size_t getRegionID(const std::string& atlas_name, 
		const std::string& region_name) const;
	std::size_t getRegionID(std::size_t atlas_id, 
		const std::string& region_name) const;

	//loads the next image file in tex_load_queue
	//also loads its atlas if one exists
	void loadNextImage();
	//loads a texture from the image_buf
	//and parses the atlas string to a map of rectangles
	void loadNextTex();

	//check if the given region or atlas tex is loaded yet
	bool isRegionLoaded(std::size_t atlas_id, std::size_t region_id) const;
	bool isRegionLoaded(const std::string& atlas_name, 
						const std::string& region_name) const;
	bool isTexLoaded(std::size_t atlas_id) const;
	bool isTexLoaded(const std::string& atlas_name) const;

	//get a sprite object from the data, provided it exists in memory
	ResSprite constructSprite(const std::string& atlas_name,
		const std::string& region_name,
		std::size_t num_frames,
		Vector2 size);
	ResSprite constructSprite(const std::string& atlas_name,
		Vector2 size);

	std::array<
		ResBuf<Rectangle, Res::REGION_BUF_SIZE>,
		Res::TEX_BUF_SIZE
	>& getRegionBufs() { return region_bufs; }
	Rectangle getRegionAt(std::size_t atlas_id, std::size_t region_id);
	Rectangle getRegionAt(std::size_t atlas_id, 
		const std::string& region_name);
	Rectangle getRegionAt(const std::string& atlas_name, 
		const std::string& region_name);
	std::vector<std::string> getRegionNames(
		const std::string& atlas_name) const;
	ResBuf<Texture2D, Res::TEX_BUF_SIZE>& getTexBuf() {return tex_buf;}
	Texture2D getTexAt(std::size_t atlas_id);

	~ManTex();
};

#endif /* SRC_RESMAN_H_ */
