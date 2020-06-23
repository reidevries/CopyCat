/*
 * example.h
 *
 *  Created on: Jun 13, 2020
 *      Author: rei de vries
 *		This is an example implementation of GameObject
 *		used to test the message parsing and update features
 *		It increments a timer every update and occasionally sends
 *		a system message to display the timer value. It can receive
 *		system messages to set the data to a number or reset it to 0.
 */

#ifndef SRC_GAMEOBJECTS_EXAMPLE_H_
#define SRC_GAMEOBJECTS_EXAMPLE_H_

#include "raylib.h"
#include "../gameobject.h"
#include "../messagelist.h"
#include <vector>
#include <string>

class Example : public GameObject
{
private:
	float my_example_data = 0;

	//below declaration not necessary,
	//but recommended for clarity and to look pretty
	static const std::string words[];
	enum Key{set, reset};

	void parseMessage(Message message, std::vector<Token> lexed);
public:
	Example();
	std::vector<Message> update(CatClock& clk);
};

#endif /* SRC_GAMEOBJECTS_EXAMPLE_H_ */
