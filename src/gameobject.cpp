#include "gameobject.h"

using namespace std;

int GameObject::id_counter = 1;


GameObject::GameObject(
	const string set_name,
	const std::vector<std::string> set_keywords)
	: keywords(set_keywords), id(id_counter++), name(set_name)
{
}

GameObject::GameObject(
	const string set_name,
	vector<unique_ptr<TexSprite>>& sprites,
	Vector2 pos,
	const std::vector<std::string> set_keywords)
	: GameObject(set_name, set_keywords)
{
	replaceSprites(sprites);
	this->pos = pos;
}

GameObject::GameObject(
	const string set_name,
	unique_ptr<TexSprite> sprite,
	Vector2 pos,
	const std::vector<std::string> set_keywords)
	: GameObject(set_name, set_keywords)
{
	this->sprites.push_back(move(sprite));
	this->pos = pos;
}

int GameObject::getRenderDistance() const
{
	return pos.x-pos.y;
}

void GameObject::replaceSprites(vector<unique_ptr<TexSprite>>& new_sprites)
{
	sprites.clear();
	for (auto& sprite : new_sprites) {
		sprites.push_back(move(sprite));
	}
}

string GameObject::getInfo()
{
	stringstream ss;
	ss << id
		<< "-" << name
		<< " (" << pos.x
		<< "x" << pos.y
		<< "x" << up_pos
		<< ", " << getSpriteNum() << " sprites)";
	return ss.str();
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

void GameObject::draw(const Camera cam) {
	for (auto& s : sprites) {
		s->draw(pos, cam);
	}
}
