#include "DiceRoll.hpp"

using namespace std;

auto DiceRoll::seed = rd() ^ (
		static_cast<std::mt19937::result_type>(
			std::chrono::duration_cast<std::chrono::seconds>(
				std::chrono::system_clock::now().time_since_epoch()
			).count()
		) + static_cast<std::mt19937::result_type>(
			std::chrono::duration_cast<std::chrono::microseconds>(
				std::chrono::high_resolution_clock::now().time_since_epoch()
			).count() )
		);
