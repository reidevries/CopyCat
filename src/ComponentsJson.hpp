/*
 * SaveComponents.h
 *
 *  Created on: Jul 21, 2020
 *      Author: rei de vries
 *      Class for saving/loading component/entities to JSON files
 */

#ifndef SRC_COMPONENTSJSON_H_
#define SRC_COMPONENTSJSON_H_

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>

#include <entt/entity/registry.hpp>

#include "CatConf.hpp"
#include "IncludeComponents.hpp"
#include "IncludeJson.hpp"
#include "DebugPrinter.hpp"
#include "VectorMath.hpp"
#include "Quad.hpp"
#include "ComponentNames.hpp"
#include "ComponentSerialize.hpp"
#include "Factory.hpp"

class ComponentsJson
{
private:
	static const std::string level_directory;

	//specialised entity parsers
	static json fromFloor(entt::registry&, const entt::entity);

public:
	static json loadLevel(entt::registry& reg, std::string file_name);
	static void saveLevel(entt::registry& reg, std::string file_name);
	static void test();
};

#endif /* SRC_COMPONENTSJSON_H_ */
