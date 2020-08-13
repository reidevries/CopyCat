/*
 * resconstants.h
 *
 *  Created on: Jul 12, 2020
 *      Author: rei de vries
 */

#ifndef SRC_RESCONSTANTS_H_
#define SRC_RESCONSTANTS_H_

namespace Res {
	static const int TEX_BUF_SIZE = 32;
	static const int REGION_BUF_SIZE = 64;
	static const int AUDIO_BUF_SIZE = 512;
	static const int MAX_ANIM_FRAMES = 16;
	static const int MAX_MULTI_SPRITES = 4;

	static constexpr int MAX_ANIM_BITS = []() -> int {
		int i = 1;
		int bits = 0;
		while (MAX_ANIM_FRAMES >= i) {
			i *= 2;
			++bits;
		}
		return bits;
	} ();
}


#endif /* SRC_RESCONSTANTS_H_ */
