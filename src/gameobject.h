#ifndef GAMEOBJECT_H	
#define GAMEOBJECT_H

#include <raylib.h>
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

public:
	const int id;
	const std::string name;

	GameObject(std::string type_name,
		std::vector<TexSprite> sprites,
		int sprite_num,
		Vector2 pos);
	GameObject(std::string type_name, Vector2 pos);
	int getSpriteNum() {return sprite_num;}
	Vector2 getPos() {return pos;}
	int getUpPos() {return up_pos;}
	int getRenderDistance();

	void replaceSprites(std::vector<TexSprite> sprites, int sprite_num);
	void setPos(Vector2 pos) {this->pos = pos;}
	void setUpPos(int up_pos) {this->up_pos = up_pos;}

	virtual void passMessages(std::vector<Message>) =0;
	virtual std::vector<Message> update(float dt, unsigned int time_s) =0;
	virtual void draw() =0;
	virtual ~GameObject() {sprites.clear();}
};

#endif
