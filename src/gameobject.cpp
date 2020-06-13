#include "gameobject.h"

using namespace std;

int GameObject::id_counter = 0;

GameObject::GameObject(
	string type_name,
	vector<TexSprite> sprites,
	int sprite_num,
	Vector2 pos) : id(id_counter++), name(type_name)
{
	this->sprites = sprites;
	this->sprite_num = sprite_num;
	this->pos = pos;
}

GameObject::GameObject(
	string type_name,
	Vector2 pos) : id(id_counter++), name(type_name)
{
	this->sprites = vector<TexSprite>();
	this->sprite_num = 0;
	this->pos = pos;
}

int GameObject::getRenderDistance()
{
	return pos.x-pos.y;
}

void GameObject::replaceSprites(vector<TexSprite> sprites, int sprite_num)
{
	this->sprites = sprites;
	this->sprite_num = sprite_num;
}

