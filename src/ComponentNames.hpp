/*
 * ComponentNames.h
 *
 *  Created on: Jul 25, 2020
 *      Author: rei de vries
 *      convenient methods for adding components to entities based on json
 *      ComponentToFromJson.cpp contains auto generated
 *      getToJson/getFromJson non-template version implementations
 */

#ifndef SRC_COMPONENNAMES_H_
#define SRC_COMPONENNAMES_H_

#include <entt/entity/registry.hpp>
#include "IncludeJson.hpp"

#include "ComponentIncludes.hpp"
#include "ComponentSerialize.hpp"

namespace ReiDV
{
template<class T>
void getToJson(entt::registry& reg,
	const entt::entity entity,
	json& j,
	std::string type_text)
{
	if (reg.has<T>(entity)) {
		j[type_text] = reg.get<T>(entity);
	}
}

//call above method based on type_text only
void getToJson(entt::registry& reg,
	const entt::entity entity,
	json& j,
	std::string type_text);

template<class T>
void getFromJson(entt::registry& reg,
	const entt::entity entity,
	const json j,
	std::string type_text)
{
	if (j.contains(type_text)) {
		reg.emplace<T>(entity, j.at(type_text).get<T>());
	}
}

void getFromJson(entt::registry& reg,
	const entt::entity entity,
	const json j,
	std::string type_text);

json entityToJson(entt::registry&, const entt::entity);
json jsonToEntity(entt::registry&, const entt::entity);

}

#endif /* SRC_COMPONENTNAMES_H_ */
