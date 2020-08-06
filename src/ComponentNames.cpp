/*
 * ComponentNames.cpp
 * Generated from generator_in_files/ComponentNames.cpp.in
 * This file should be automatically generated by the python script
 * generate_component_names.py. It contains text to function mappings for
 * serializing each component to/from json
 */

#include "ComponentNames.hpp"

using namespace std;

void ReiDV::getToJson(entt::registry& reg,
	const entt::entity entity,
	json& j,
	string type_text)
{
	if (type_text == "HoverSound") {
		getToJson<HoverSound>(reg, entity, j, "HoverSound");
	} else if (type_text == "WorldPos") {
		getToJson<WorldPos>(reg, entity, j, "WorldPos");
	} else if (type_text == "SpriteSizeRot") {
		getToJson<SpriteSizeRot>(reg, entity, j, "SpriteSizeRot");
	} else if (type_text == "SpriteQuad") {
		getToJson<SpriteQuad>(reg, entity, j, "SpriteQuad");
	} else if (type_text == "SpriteAnim") {
		getToJson<SpriteAnim>(reg, entity, j, "SpriteAnim");
	} else if (type_text == "ScreenPos") {
		getToJson<ScreenPos>(reg, entity, j, "ScreenPos");
	} else if (type_text == "LevelID") {
		getToJson<LevelID>(reg, entity, j, "LevelID");
	} else if (type_text == "HitBox") {
		getToJson<HitBox>(reg, entity, j, "HitBox");
	}
}

void ReiDV::getFromJson(entt::registry& reg,
	const entt::entity entity,
	const json j,
	string type_text)
{
	if (type_text == "HoverSound") {
		getFromJson<HoverSound>(reg, entity, j, "HoverSound");
	} else if (type_text == "WorldPos") {
		getFromJson<WorldPos>(reg, entity, j, "WorldPos");
	} else if (type_text == "SpriteSizeRot") {
		getFromJson<SpriteSizeRot>(reg, entity, j, "SpriteSizeRot");
	} else if (type_text == "SpriteQuad") {
		getFromJson<SpriteQuad>(reg, entity, j, "SpriteQuad");
	} else if (type_text == "SpriteAnim") {
		getFromJson<SpriteAnim>(reg, entity, j, "SpriteAnim");
	} else if (type_text == "ScreenPos") {
		getFromJson<ScreenPos>(reg, entity, j, "ScreenPos");
	} else if (type_text == "LevelID") {
		getFromJson<LevelID>(reg, entity, j, "LevelID");
	} else if (type_text == "HitBox") {
		getFromJson<HitBox>(reg, entity, j, "HitBox");
	}
}

json ReiDV::entityToJson(entt::registry& reg, const entt::entity entity)
{
	json j;
	getToJson<HoverSound>(reg, entity, j, "HoverSound");
	getToJson<WorldPos>(reg, entity, j, "WorldPos");
	getToJson<SpriteSizeRot>(reg, entity, j, "SpriteSizeRot");
	getToJson<SpriteQuad>(reg, entity, j, "SpriteQuad");
	getToJson<SpriteAnim>(reg, entity, j, "SpriteAnim");
	getToJson<ScreenPos>(reg, entity, j, "ScreenPos");
	getToJson<LevelID>(reg, entity, j, "LevelID");
	getToJson<HitBox>(reg, entity, j, "HitBox");
	return j;
}

json ReiDV::jsonToEntity(entt::registry& reg, const entt::entity entity)
{
	json j;
	getFromJson<HoverSound>(reg, entity, j, "HoverSound");
	getFromJson<WorldPos>(reg, entity, j, "WorldPos");
	getFromJson<SpriteSizeRot>(reg, entity, j, "SpriteSizeRot");
	getFromJson<SpriteQuad>(reg, entity, j, "SpriteQuad");
	getFromJson<SpriteAnim>(reg, entity, j, "SpriteAnim");
	getFromJson<ScreenPos>(reg, entity, j, "ScreenPos");
	getFromJson<LevelID>(reg, entity, j, "LevelID");
	getFromJson<HitBox>(reg, entity, j, "HitBox");
	return j;
}
