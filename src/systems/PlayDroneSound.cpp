/*
 * Created: 02/08 2020
 *  by Rei de Vries
 */

#include "Systems.hpp"

using namespace std;
using namespace VectorMath;

float Systems::calcOctaveReducePitch(Fraction ji) {
	float f = ji.getf();
	if (octave_reduce <= 2 && f > 8.0f) {
		return 0.125f;
	} else if (octave_reduce <= 1 && f > 4.0f) {
		return 0.25f;
	} else if (octave_reduce == 0 && f > 2.0f) {
		return 0.5f;
	}
	return 1.0f;
}

void Systems::playDroneSound(entt::registry& reg, 
	const CatClock& clk,
	ManAudio& man_audio)
{
	if (!clk.tock_beat) return;
	uint8_t new_octave_reduce = octave_reduce;
	if (num_active_hills < 5) new_octave_reduce = 0;
	else if (num_living_bats == num_active_hills) new_octave_reduce = 3;
	else if (num_living_bats > num_active_hills*0.666f) new_octave_reduce = 2;
	else if (num_living_bats > num_active_hills*0.333f) new_octave_reduce = 1;
	if (octave_reduce != new_octave_reduce) {
		stringstream ss;
		ss << "New octave reduce value of " 
			<< static_cast<int>(new_octave_reduce) << endl;
		DebugPrinter::printDebug(4, "Systems::playDroneSound", ss.str());
		octave_reduce = new_octave_reduce;
	}
	
	auto view = reg.view<DroneSound>();

	float limiting = 0;
	for (const entt::entity e : view) {
		ResSound sound = view.get<DroneSound>(e).sound;
		limiting += sound.vol;
	}
	
	limiting = 3.0f/(2.0f+limiting);//values of 3 and 2 approximate sqrt(1/l)
	if (limiting > 1.0f) limiting = 1.0f;
	
	for (const entt::entity e : view) {
		DroneSound drone_sound = view.get<DroneSound>(e);
		float repitch = calcOctaveReducePitch(drone_sound.ji);
		if (drone_sound.sound.vol >= CLOSE_ENUF*2) {
			man_audio.playSound(drone_sound.sound, limiting, repitch);
		}
	}
}
