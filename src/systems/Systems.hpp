/*
 * Created: 02/08 2020
 *  by Rei de Vries
 *	defines each system, implementation in separate file
 */

#ifndef SYSTEMS_H
#define SYSTEMS_H

#include <optional>

#include <entt/entity/registry.hpp>

#include "CatConf.hpp"
#include "../CatClock.hpp"
#include "../InputData.hpp"
#include "../IncludeComponents.hpp"
#include "../VectorMath.hpp"
#include "../ManAudio.hpp"
#include "../DebugPrinter.hpp"

//System for playing sound when the mouse hovers over an entity 
//that has a "HoverSound" component
class Systems
{
private:
	std::optional<entt::entity> sound_on_hover_hit;
public:
	void soundOnHover(entt::registry&, 
		const InputData::Mouse&, 
		ManAudio&);
	void growOnPress(entt::registry&,
		const CatClock& clk,
		const InputData::Mouse&);
	void playDroneSound(entt::registry&,
		ManAudio&);
};

#endif // SOUNDONHOVER_H
