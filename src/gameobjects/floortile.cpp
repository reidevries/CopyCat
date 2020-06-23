/*
 * floortile.cpp
 *
 *  Created on: Jun 13, 2020
 *      Author: rei de vries
 */

#include "floortile.h"

using namespace std;

const string FloorTile::words[] = {
	"next"
};
const string FloorTile::type_text[] = {
	"floor",
	"lava"
};

FloorTile::FloorTile(FloorTile::Type floor_type,
	vector<unique_ptr<TexSprite>>& textures)
	: GameObject(
		"FloorTile",
		textures,
		{10,10},
		std::vector<std::string>(begin(words), end(words))
	)
{
	this->floor_type = floor_type;
}

FloorTile::FloorTile(FloorTile::Type floor_type,
	unique_ptr<TexSprite> texture)
	: GameObject(
		"FloorTile",
		move(texture),
		{10,10},
		std::vector<std::string>(begin(words), end(words))
	)
{
	this->floor_type = floor_type;
}

void FloorTile::parseMessage(Message message, std::vector<Token> lexed)
{
	switch (message.type) {
	case Message::Type::inspect:
	{
		stringstream ss;
		ss << "This is some " << type_text[floor_type];
		message_buf.emplace_back(id,
			message.src_id,
			Message::Type::inspect_response,
			ss.str());
		break;
	}
	case Message::Type::say:
	{
		for (auto const& tok : lexed) {
			switch(static_cast<Key>(tok.keyword_index)) {
			case next:
				++sprite_index;
				if (sprite_index >= sprites.size()) {
					sprite_index = 0;
				}
				break;
			default:
				break;
			}
		}
		break;
	}
	default:
		break;
	}
}

vector<Message> FloorTile::update(CatClock& clk)
{
	//move around the floor in a zigzag pattern
	pos.x = 10*(clk.time_s-30);
	pos.y = clk.time_ms-500;
	pos.y *= 0.2;
	vector<Message> response(message_buf);
	message_buf.clear();
	return response;
}
