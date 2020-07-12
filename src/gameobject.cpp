#include "gameobject.h"

using namespace std;

uint16_t GameObject::id_counter = 1;


GameObject::GameObject(
	const string set_name,
	const std::vector<std::string> set_keywords)
	: keywords(set_keywords), id(id_counter++), name(set_name)
{
}

GameObject::GameObject(
	const string set_name,
	vector<TexSprite>& sprites,
	Vector2 pos,
	const std::vector<std::string> set_keywords)
	: GameObject(set_name, set_keywords)
{
	this->sprites = sprites;
	this->pos = pos;
}

GameObject::GameObject(
	const string set_name,
	TexSprite sprite,
	Vector2 pos,
	const std::vector<std::string> set_keywords)
	: GameObject(set_name, set_keywords)
{
	this->sprites.push_back(sprite);
	this->pos = pos;
}

int GameObject::getRenderDistance() const
{
	return pos.x-pos.y;
}

void GameObject::rotateSprites(float roll, float pitch, float yaw)
{
	for (auto& s : sprites) {
		s.rotQuad(roll,pitch,yaw);
	}
}

string GameObject::getInfo() const
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

void GameObject::draw(ResBuf<Texture2D, Res::TEX_BUF_SIZE>& tex_buf,
	std::array<
		ResBuf<Rectangle, Res::REGION_BUF_SIZE>,
		Res::TEX_BUF_SIZE
	>& region_bufs,
	Camera cam) {
	for (auto& s : sprites) {
		int cur_id = s.getResID();
		int cur_region = s.getCurRegionID();
		//only draw if the region rectangle is loaded
		if (!region_bufs[cur_id].isFree(cur_region)) {
			Rectangle src_rect = region_bufs[cur_id].at(cur_region);
			switch (s.getType()) {
			case (TexSprite::Type::billboard):
				s.drawBillboard(tex_buf.at(s.getResID()),
					src_rect,
					pos,
					cam);
				break;
			case (TexSprite::Type::world):
				s.drawWorld(tex_buf.at(s.getResID()),
					src_rect,
					pos,
					cam);
				break;
			case (TexSprite::Type::screen):
				s.drawScreen(tex_buf.at(s.getResID()),
					src_rect,
					WHITE);
				break;
			}
		}
	}
}
