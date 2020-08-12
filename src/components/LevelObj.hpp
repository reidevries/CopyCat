/*
 * LevelObj.hpp
 *
 *  Created on: Jul 25, 2020
 *      Author: rei de vries
 *
 *      This component indicates that an entity should be saved as part of the
 *      level or loaded as part of a level, as opposed to being created
 *      while the game is running. It contains a string-format id
 */

#ifndef SRC_COMPONENTS_LEVELOBJ_H_
#define SRC_COMPONENTS_LEVELOBJ_H_

#include <string>

struct LevelObj {
	std::string name;
};

#endif /* SRC_COMPONENTS_LEVELOBJ_H_ */
