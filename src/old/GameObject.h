#ifndef GAMEOBJECT_H	
#define GAMEOBJECT_H

#include <algorithm>
#include <vector>
#include <sstream>
#include <memory>
#include <cstdint>

#include "../Level.h"
#include "../MessageList.h"
#include "../ResBuf.h"
#include "../ResConstants.h"
#include "../TexSprite.h"
#include "catconf.h"
#include "CatClock.h"

class GameObject
{
private:
	static uint16_t id_counter;

protected:
	std::vector<TexSprite> sprites;
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
	const uint16_t id;
	const std::string name;

	GameObject(const std::string type_name,
		const std::vector<std::string> set_keywords);
	GameObject(const std::string type_name,
		std::vector<TexSprite>& sprites,
		const Vector2 pos,
		const std::vector<std::string> set_keywords);
	GameObject(const std::string set_name,
		TexSprite sprite,
		const Vector2 pos,
		const std::vector<std::string> set_keywords);

	std::vector<TexSprite>& getSprites() {return sprites;}
	int getSpriteNum() const {return sprites.size();}
	Vector2 getPos() const {return pos;}
	int getUpPos() const {return up_pos;}
	int getRenderDistance() const;

	void setPos(Vector2 pos) {this->pos = pos;}
	void setUpPos(int up_pos) {this->up_pos = up_pos;}
	void rotateSprites(float roll, float pitch, float yaw);

	std::string getInfo() const;

	virtual void passMessages(std::vector<Message>);

	//implement these methods to create a gameobject
	virtual std::vector<Message> update(CatClock& clk) =0;
	virtual void draw(ResBuf<Texture2D, Res::TEX_BUF_SIZE>& tex_buf,
		std::array<
			ResBuf<Rectangle, Res::REGION_BUF_SIZE>,
			Res::TEX_BUF_SIZE
		>& region_bufs,
		Camera cam);
	virtual ~GameObject() {sprites.clear();}
};

#endif
