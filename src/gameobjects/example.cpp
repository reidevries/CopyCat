/*
 * example.cpp
 *
 *  Created on: Jun 13, 2020
 *      Author: rei de vries
 */

#include "example.h"

using namespace std;

Example::Example()
		: GameObject(
			"Example",
			vector<TexSprite>(),
			0,
			{0,0},
			{"set", "reset"}
		)
{
}

void Example::parseMessage(Message message, std::vector<Token> lexed)
{
	switch (message.type) {
	case Message::Type::say:
		for (auto const& tok : lexed) {
			DebugPrinter::printDebug("Example::parseMessage", keywords[tok.keyword_index], 4);
			switch(static_cast<Key>(tok.keyword_index)) {
			case set:
				my_example_data = tok.numeral;
				break;
			case reset:
				my_example_data = 0;
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
}

vector<Message> Example::update(const float dt,
	const unsigned int time_s,
	const unsigned int tick)
{
	my_example_data += dt;
	vector<Message> response;
	if (tick == 1) response.emplace_back(id,0,Message::Type::debug,to_string(my_example_data));
	return response;
}

void Example::draw()
{
}
