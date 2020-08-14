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
#include "../DiceRoll.hpp"
#include "../CatClock.hpp"
#include "../InputData.hpp"
#include "../IncludeComponents.hpp"
#include "../VectorMath.hpp"
#include "../ManAudio.hpp"
#include "../ManTex.hpp"
#include "../DebugPrinter.hpp"
#include "../Fraction.hpp"
#include "../WorldConstants.hpp"
#include "../Factory.hpp"


class Systems
{
private:
	DiceRoll dice;
	//the entity currently being dragged by the mouse
	//managed by growOnPress
	std::optional<entt::entity> e_dragged;
	//the entity with a hoversound currently hit by mouse ray
	//set by soundOnHover
	std::optional<entt::entity> sound_on_hover_hit;
	
	std::size_t num_active_hills = 0;
	std::size_t num_living_bats = 0;
	
	//number of octaves allowed in output
	//3 max means no octave reduction takes place
	//0 means all octaves lowered to 1st octave
	uint8_t octave_reduce = 0;
	
	void seekWater(entt::registry& reg, Bat& bat, const Vector3& bat_pos);
	void seekHill(entt::registry& reg, Bat& bat, const Vector3& bat_pos);
	float calcOctaveReducePitch(Fraction ji);

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
	void velocity(entt::registry&,
		const CatClock& clk);
	void spawnCreatures(entt::registry&,
		const CatClock& clk,
		ManTex&,
		ManAudio&);
	void batAI(entt::registry&,
		const CatClock& clk,
		const std::array<std::array<Fraction,World::SIZE_Y>,World::SIZE_X>& ji,
		ManAudio&);
};

#endif // SOUNDONHOVER_H
