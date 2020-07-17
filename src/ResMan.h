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

#include <vector>
#include <queue>
#include <map>
#include <string>
#include <memory>
#include <sstream>
#include <cstdint>

#include "catconf.h"
#include "debugprinter.h"
#include "catclock.h"
#include "texsprite.h"
#include "resbuf.h"
#include "resconstants.h"

class ResMan {
private:
	static const std::string tex_directory;

	std::string constructAtlasFilename(
		std::string) const;
	std::string constructImageFilename(
		std::string) const;

	const bool debug;

	std::queue<std::string> tex_load_queue;
	//buffer for storing image partway through loading
	struct ImageBuf {
		//if ID >= Res::TEX_BUF_SIZE, buf is empty
		uint8_t ID = Res::TEX_BUF_SIZE;
		std::string name;
		Image image;
		Rectangle rect;
		std::string atlas;
	};
	ImageBuf image_buf;
	//used for parsing atlas file, based on .rtpa format
	enum AtlasTok {s,		nameID,
		originX, 			originY,
		positionX, 			positionY,
		sourceSizeWidth, 	sourceSizeHeight,
		padding,			trimmed,
		trimRecX,			trimRecY,
		trimRecWidth,		trimRecHeight
	};

	ResBuf<Texture2D, Res::TEX_BUF_SIZE> tex_buf;
	std::array<ResBuf<Rectangle, Res::REGION_BUF_SIZE>, Res::TEX_BUF_SIZE> region_bufs;

	//store a map of string to Rectangle in the region buf
	void storeAtlas(std::string atlas_name, std::map<std::string, Rectangle>);
	void parseAtlasString(std::string atlas);

	void freeTexByIndex(uint8_t index);
	uint8_t requestTex(std::string name);
	std::vector<uint8_t> requestRegions(uint8_t atlas_id,
		std::string name, uint8_t num_frames);
	uint8_t requestRegion(uint8_t atlas_id, std::string name);

public:
	ResMan(const bool set_debug);
	ResMan();

	uint8_t getAtlasID(std::string name);
	uint8_t getRegionID(std::string atlas_name, std::string region_name);

	//loads the next image file in tex_load_queue
	//also loads its atlas if one exists
	void loadNextImage();
	//loads a texture from the image_buf
	//and parses the atlas string to a map of rectangles
	void loadNextTex();

	//check if the given region or atlas tex is loaded yet
	bool isRegionLoaded(uint8_t atlas_id, uint8_t region_id) const;
	bool isRegionLoaded(std::string atlas_name,std::string region_name) const;
	bool isTexLoaded(uint8_t atlas_id) const;
	bool isTexLoaded(std::string atlas_name) const;

	//get a sprite object from the data, provided it exists in memory
	TexSprite constructSprite(std::string atlas_name,
		std::string region_name,
		uint8_t num_frames,
		TexSprite::Type type,
		Vector2 size);
	TexSprite constructSprite(std::string atlas_name,
		TexSprite::Type,
		Vector2 size);

	std::array<
		ResBuf<Rectangle, Res::REGION_BUF_SIZE>,
		Res::TEX_BUF_SIZE
	>& getRegionBufs() { return region_bufs; }
	Rectangle getRegionAt(uint8_t atlas_id, uint8_t region_id);
	ResBuf<Texture2D, Res::TEX_BUF_SIZE>& getTexBuf() {return tex_buf;}
	Texture2D getTexAt(uint8_t atlas_id);

	~ResMan();
};

#endif /* SRC_RESMAN_H_ */
