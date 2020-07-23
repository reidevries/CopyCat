/*
 * SaveComponents.h
 *
 *  Created on: Jul 21, 2020
 *      Author: rei de vries
 *      Class for saving/loading component/entities to JSON files
 */

#ifndef SRC_JSONCOMPONENTS_H_
#define SRC_JSONCOMPONENTS_H_

#include <string>
#include <fstream>
#include <iostream>

#include <entt/entity/registry.hpp>

#include "CatConf.h"
#include "IncludeJson.h"
#include "DebugPrinter.h"
#include "VectorMath.h"
#include "Quad.h"
#include "ComponentNames.h"
#include "ComponentList.h"
#include "ResMan.h"

class JsonComponents
{
private:
	json loadLevel(entt::registry& reg,
		ResMan& res_man, std::string file_name);
	void saveLevel(entt::registry& reg, std::string file_name);

public:
	static void test();
};

#endif /* SRC_JSONCOMPONENTS_H_ */
