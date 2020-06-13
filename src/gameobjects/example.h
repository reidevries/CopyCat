/*
 * example.h
 *
 *  Created on: Jun 13, 2020
 *      Author: raeaw
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

	enum Key{set, reset}; //not necessary, but makes code pretty

	void parseMessage(Message message, std::vector<Token> lexed);
public:
	Example();
	std::vector<Message> update(const float dt,
		const unsigned int time_s,
		const unsigned int tick);
	void draw();
};

#endif /* SRC_GAMEOBJECTS_EXAMPLE_H_ */
