/*
 * Created: 02/08 2020
 *  by Rei de Vries
 */

#include "Systems.hpp"

using namespace std;
using namespace VectorMath;

void Systems::playDroneSound(entt::registry& reg, 
	const CatClock& clk,
	ManAudio& man_audio)
{
	if (!clk.tock_beat) return;
	
	auto view = reg.view<DroneSound>();

	float limiting = 0;
	for (const entt::entity e : view) {
		ResSound sound = view.get<DroneSound>(e).sound;
		limiting += sound.vol;
	}
	
	limiting = 3.0f/(2.0f+limiting);//values of 3 and 2 approximate sqrt(1/l)
	if (limiting > 1.0f) limiting = 1.0f;
	
	for (const entt::entity e : view) {
		ResSound sound = view.get<DroneSound>(e).sound;
		if (sound.vol >= CLOSE_ENUF*2) {
			man_audio.playSound(sound, limiting);
		}
	}
}
