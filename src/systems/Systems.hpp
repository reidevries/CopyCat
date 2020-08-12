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


class Systems
{
private:
	//the entity currently being dragged by the mouse
	//managed by growOnPress
	std::optional<entt::entity> e_dragged;
	//the entity with a hoversound currently hit by mouse ray
	//set by soundOnHover
	std::optional<entt::entity> sound_on_hover_hit;

public:
	//System for playing sound when the mouse hovers over an entity 
	//that has a "HoverSound" component. Need to do this before growOnPress
	//ideally
	void soundOnHover(entt::registry&, 
		const InputData::Mouse&, 
		ManAudio&);
	//grows an the quad of an entity with a growsound with a parent with a
	//hoversound
	void growOnPress(entt::registry&,
		const CatClock& clk,
		const InputData::Mouse&);
	//plays drone sounds
	void playDroneSound(entt::registry&,
		const CatClock& clk,
		ManAudio&);
	void BatAI(entt::registry&,
		const CatClock& clk);
};

#endif // SOUNDONHOVER_H
