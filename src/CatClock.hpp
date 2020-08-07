/*
 * CatClock.hpp
 *
 *  Created on: Jun 22, 2020
 *      Author: rei de vries
 *		Just a struct for timing data
 */

#ifndef SRC_CATCLOCK_H_
#define SRC_CATCLOCK_H_

#include <cstdint>

struct CatClock {
	float dt_s;				//time in seconds since last frame
	uint16_t time_m;		//time in minutes since process began, max 45 days
	uint8_t time_s;			//time in seconds since last minute tick, max 60
	float time_ms;			//time in ms since last second tick, max 1000
	// time in ms since start of last beat
	float time_beat;

	uint32_t time_f;		//time in frames since process began

	//these become true only for the frame at which exactly a minute
	//or exactly a second has passed, so that a task can be done once
	//every minute or once every second without having to keep record
	//in the object itself.
	bool tock_m;
	bool tock_s;
	bool tock_beat;

	//constants for converting between units
	static const int MS_S = 1000;
	static const int S_M = 60;
	static constexpr float S_MS = 1/MS_S;
	static constexpr float M_S = 1/S_M;
	static const int MSPB = 68;                               // ms per beat

	//call this at the beginning of every frame to update the clock
	void tick(float dt_s) {
		this->dt_s = dt_s;
		tock_m = false;
		tock_s = false;
		tock_beat = false;
		
		time_beat += dt_s*MS_S;
		if (time_beat >= MSPB) {
			time_beat = 0;
			tock_beat = true;
		}

		++time_f;

		time_ms += dt_s*MS_S;
		while (time_ms >= MS_S) {
			time_ms -= MS_S;
			++time_s;
			tock_s = true;

			if (time_s >= S_M) {
				time_s = 0;
				++time_m;
				tock_m = true;
			}
		}
	}

	//gets time since process began as a double with millisecond resolution
	double total_time_m() {
		return (time_ms*S_MS+time_s)*M_S + time_m;
	}
	double total_time_ms() {
		return (time_m*S_M+time_s)*MS_S + time_ms;
	}

	//returns the frames per second as int
	int fps() {
		return 1/dt_s;
	}
};



#endif /* SRC_TIMING_H_ */
