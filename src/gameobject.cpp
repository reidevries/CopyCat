#include "gameobject.h"

using namespace std;

int GameObject::id_counter = 1;

GameObject::GameObject(
	string type_name,
	vector<TexSprite> sprites,
	int sprite_num,
	Vector2 pos,
	vector<string> set_keywords)
	: keywords(set_keywords), id(id_counter++), name(type_name)
{
	this->sprites = sprites;
	this->sprite_num = sprite_num;
	this->pos = pos;
}

GameObject::GameObject(
	string type_name,
	Vector2 pos,
	vector<string> set_keywords)
	: keywords(set_keywords), id(id_counter++), name(type_name)
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

void GameObject::parseMessage(Message, vector<Token> lexed)
{
}

void GameObject::passMessages(vector<Message> messages)
{
	for (auto const& message : messages) {
		stringstream ss(message.message);
		string buf;
		vector<Token> lexed;
		while (ss >> buf) {
			Token tok = {-1,-1};
			if (any_of(buf.begin(), buf.end(), ::isdigit)) {
				if (lexed.size() > 0) {
					lexed[lexed.size()-1].numeral = stoi(buf);
				} else {
					tok.numeral = stoi(buf);
					lexed.push_back(tok);
				}
			}

			for (int i = 0; i < keywords.size(); ++i) {
				if (keywords[i] == buf) {
					tok.keyword_index = i;
					lexed.push_back(tok);
					break;
				}
			}
		}
		parseMessage(message, lexed);
	}
}
