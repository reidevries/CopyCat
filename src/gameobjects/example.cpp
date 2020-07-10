/*
 * example.cpp
 *
 *  Created on: Jun 13, 2020
 *      Author: rei de vries
 */

#include "example.h"

using namespace std;

const string Example::words[] = {
	"set",
	"reset",
	"spin"
};

Example::Example(TexSprite tex)
		: GameObject(
			"Example",
			tex,
			{10,10},
			std::vector<string>(begin(words), end(words))
		)
{
}

void Example::parseMessage(Message message, std::vector<Token> lexed)
{
	switch (message.type) {
	case Message::Type::say:
		for (auto const& tok : lexed) {
			switch(static_cast<Key>(tok.keyword_index)) {
			case set:
				my_example_data = tok.numeral;
				break;
			case reset:
				my_example_data = 0;
				break;
			case spin:
				rotateSprites((Vector3){10,1000,100});
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
}

vector<Message> Example::update(CatClock& clk)
{
	my_example_data += clk.dt_s;
	vector<Message> response;
	if (clk.tock_m) response.emplace_back(id,0,Message::Type::debug,to_string(my_example_data));
	return response;
}
