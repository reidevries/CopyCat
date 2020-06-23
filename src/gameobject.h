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
#include "catclock.h"

class GameObject
{
private:
	static int id_counter;

protected:
	std::vector<std::unique_ptr<TexSprite>> sprites;
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
	virtual void parseMessage(Message, std::vector<Token>);
public:
	const int id;
	const std::string name;

	GameObject(const std::string type_name,
		const std::vector<std::string> set_keywords);
	GameObject(const std::string type_name,
		std::vector<std::unique_ptr<TexSprite>>& sprites,
		const Vector2 pos,
		const std::vector<std::string> set_keywords);
	GameObject(const std::string set_name,
		std::unique_ptr<TexSprite> sprite,
		const Vector2 pos,
		const std::vector<std::string> set_keywords);

	int getSpriteNum() const {return sprites.size();}
	Vector2 getPos() const {return pos;}
	int getUpPos() const {return up_pos;}
	int getRenderDistance() const;

	void replaceSprites(std::vector<std::unique_ptr<TexSprite>>& sprites);
	void setPos(Vector2 pos) {this->pos = pos;}
	void setUpPos(int up_pos) {this->up_pos = up_pos;}

	std::string getInfo();

	virtual void passMessages(std::vector<Message>);

	//implement these methods to create a gameobject
	virtual std::vector<Message> update(CatClock& clk) =0;
	virtual void draw(Camera cam);
	virtual ~GameObject() {sprites.clear();}
};

#endif
