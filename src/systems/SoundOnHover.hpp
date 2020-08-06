/*
 * Created: 02/08 2020
 *  by Rei de Vries
 */

#ifndef SOUNDONHOVER_H
#define SOUNDONHOVER_H

#include <entt/entity/registry.hpp>

#include "CatConf.hpp"
#include "../InputData.hpp"
#include "../components/HoverSound.hpp"
#include "../components/SpriteQuad.hpp"
#include "../components/WorldPos.hpp"
#include "../ResBuf.hpp"
#include "../VectorMath.hpp"
#include "../DebugPrinter.hpp"

//System for playing sound when the mouse hovers over an entity 
//that has a "HoverSound" component
namespace Systems
{
void soundOnHover(entt::registry& reg, 
	const InputData::Mouse& mouse, 
	const ResBuf<Sound, Res::AUDIO_BUF_SIZE>& audio_buf);
}

#endif // SOUNDONHOVER_H
