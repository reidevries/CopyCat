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

class ResMan {
private:
	static const std::string tex_directory;

	std::string constructAtlasFilename(
		std::string) const;
	std::string constructImageFilename(
		std::string) const;

	const bool debug;

	//	-	-	TEXTURE BUFFER SECTION	-	-	-	//
	std::queue<std::string> tex_load_queue;
	//buffer for storing image partway through loading
	struct ImageBuf {
		std::string name;
		Image image;
		std::string atlas;
		//if ID >= MAX_BUF_SIZE, buf is empty
		uint8_t ID = Res::MAX_BUF_SIZE;
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

	ResBuf<Texture2D> tex_buf;
	std::array<ResBuf<Rectangle>, Res::MAX_BUF_SIZE> region_bufs;

	//store a map of string to Rectangle in the region buf
	void storeAtlas(std::string atlas_name, std::map<std::string, Rectangle>);
	void parseAtlasString(std::string atlas);
	//	-	-	END TEXTURE BUFFER SECTION	-	-	//

	void freeTexByIndex(uint8_t index);
	uint8_t newTex(std::string name);

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

	//tells the resource manager that we want a certain region
	//returns true if region is already loaded
	bool requestRegion(std::string atlas_name, std::string region_name);
	//check if the given region or atlas tex is loaded yet
	bool isRegionLoaded(std::string atlas_name,std::string region_name) const;
	bool isTexLoaded(std::string atlas_name) const;

	std::vector<uint8_t> getRegions(std::string atlas_name,
		std::string region_name) const;

	//get a sprite object from the data, provided it exists in memory
	TexSprite constructSprite(std::string atlas_name,
		std::string region_name,
		TexSprite::Type type,
		Vector2 size);
	TexSprite constructSprite(std::string atlas_name,
		TexSprite::Type,
		Vector2 size);

	ResBuf<Texture2D>& getTexBuf() {return tex_buf;}
	Texture2D getTexAt(uint8_t i);
	std::array<ResBuf<Rectangle>, Res::MAX_BUF_SIZE>& getRegionBufs()
	{
		return region_bufs;
	}
	Rectangle getRegionAt(uint8_t atlas_index, uint8_t region_index);
	Model getModelAt(uint8_t id);

	~ResMan();
};

#endif /* SRC_RESMAN_H_ */
