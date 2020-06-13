#ifndef GAMEOBJECT_H	
#define GAMEOBJECT_H

#include <raylib.h>
#include <algorithm>
#include <vector>
#include "level.h"
#include <memory>
#include "texsprite.h"
#include <sstream>
#include "messagelist.h"

class GameObject
{
private:
	static int id_counter;

protected:
	std::vector<TexSprite> sprites;
	int sprite_num;
	Vector2 pos;
	int up_pos = 0;

	//struct representing a token from the message parser
	struct Token {
		int keyword_index = -1;
		int numeral = -1;
	};

	//the parser will pick these up from the message
	const std::vector<std::string> keywords;

	//implement this to parse a message
	//the vector contains an array of lexed information
	//the array's first member is the keyword matched,
	//the second is the int following the keyword (-1 if there is none)
	virtual void parseMessage(const Message, const std::vector<Token>);
public:
	const int id;
	const std::string name;

	GameObject(std::string type_name,
		std::vector<TexSprite> sprites,
		int sprite_num,
		Vector2 pos,
		std::vector<std::string> set_keywords);
	GameObject(std::string type_name,
		Vector2 pos,
		std::vector<std::string> set_keywords);
	int getSpriteNum() {return sprite_num;}
	Vector2 getPos() {return pos;}
	int getUpPos() {return up_pos;}
	int getRenderDistance();

	void replaceSprites(std::vector<TexSprite> sprites, int sprite_num);
	void setPos(Vector2 pos) {this->pos = pos;}
	void setUpPos(int up_pos) {this->up_pos = up_pos;}

	virtual void passMessages(std::vector<Message>);

	//implement these methods to create a gameobject
	virtual std::vector<Message> update(const float dt,
		const unsigned int time_s,
		const unsigned int tick) =0;
	virtual void draw() =0;
	virtual ~GameObject() {sprites.clear();}
};

#endif
