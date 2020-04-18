#include "gameobject.h"

GameObject::GameObject(std::vector<TexSprite> sprites, int sprite_num, Vector2 pos, float screen_scale) {
	this->sprites = sprites;
	this->sprite_num = sprite_num;
	this->pos = pos;
	
	setScreenScale(screen_scale);
}

GameObject::GameObject(Vector2 pos, float screen_scale) {
	this->sprites = std::vector<TexSprite>();
	this->sprite_num = 0;
	this->pos = pos;

	setScreenScale(screen_scale);
}

void GameObject::replaceSprites(std::vector<TexSprite> sprites, int sprite_num) {
	this->sprites = sprites;
	this->sprite_num = sprite_num;
}

void GameObject::setScreenScale(float screen_scale) {
	this->screen_scale = screen_scale;
	for (auto sprite : sprites) {
		sprite.setScreenScale(screen_scale);
	}
}

std::unique_ptr<Level>  GameObject::update(std::unique_ptr<Level> level, float dt) {
	return level;
}
