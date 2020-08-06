/*
 * ComponentSerialize.cpp
 *
 *  Created on: Jul 23, 2020
 *      Author: rei de vries
 */

#ifndef SRC_COMPONENTSERIALIZE_CPP_
#define SRC_COMPONENTSERIALIZE_CPP_

#include "ComponentSerialize.hpp"

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(HoverSound, sound)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(WorldPos, pos)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SpriteSizeRot, size, rot)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SpriteQuad, quad)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SpriteAnim, atlas_name, region_name)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ScreenPos, pos)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(LevelID, name)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(HitBox, box)

#endif /* SRC_COMPONENTSERIALIZE_H_ */
