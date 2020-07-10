/*
 * floortile.h
 *
 *  Created on: Jun 13, 2020
 *      Author: rei de vries
 */

#ifndef SRC_GAMEOBJECTS_FLOORTILE_H_
#define SRC_GAMEOBJECTS_FLOORTILE_H_

#include <vector>
#include <string>

#include "catconf.h"
#include "../gameobject.h"
#include "../messagelist.h"
#include "../texsprite.h"

class FloorTile : public GameObject
{
//must declare enum first or we can't use it
public:
	enum Type{floor, lava};

private:
	enum Key{};
	static const std::string words[];

	static const std::string type_text[];
	FloorTile::Type floor_type;

	int sprite_index = 0;

	std::vector<Message> message_buf;

	void parseMessage(Message message, std::vector<Token> lexed);

public:
	FloorTile(FloorTile::Type, TexSprite);
	std::vector<Message> update(CatClock& clk);
};

#endif /* SRC_GAMEOBJECTS_FLOORTILE_H_ */
