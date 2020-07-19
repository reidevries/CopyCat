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
	TexSprite tex)
	: GameObject(
		"FloorTile",
		tex,
		{10,10},
		std::vector<string>(begin(words), end(words))
	)
{
	this->floor_type = floor_type;
}

void FloorTile::parseMessage(Message message, std::vector<Token> lexed)
{
	switch (message.type) {
	case Message::Type::inspect:
		stringstream ss;
		ss << "This is some " << type_text[floor_type];
		message_buf.emplace_back(id,
			message.src_id,
			Message::Type::inspect_response,
			ss.str());
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
