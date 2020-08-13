#include "DiceRoll.hpp"

using namespace std;

DiceRoll::DiceRoll()
{
	seed = rd() ^ (
		static_cast<mt19937::result_type>(
			chrono::duration_cast<chrono::seconds>(
				chrono::system_clock::now().time_since_epoch()
			).count()
		) + static_cast<mt19937::result_type>(
			chrono::duration_cast<chrono::microseconds>(
				chrono::high_resolution_clock::now().time_since_epoch()
		).count() )
	);

	gen = mt19937(seed);
}

float DiceRoll::rollF()
{
	return static_cast<float>(gen())/static_cast<float>(gen.max());
}
