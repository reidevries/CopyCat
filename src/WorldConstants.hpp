/*
 * WorldConstants.hpp
 *
 *  Created on: Aug 2, 2020
 *      Author: rei de vries
 *		defines global world units
 */

#ifndef SRC_WORLDCONSTANTS_H_
#define SRC_WORLDCONSTANTS_H_

namespace World
{
	// 1 metre is 20 "world pixels"
	static const int METRE = 18;
	// world size is 128x128 metres?? (what does this mean?)
	static const int HEIGHT = 128;
	// bucket size is 8x8 metres
	static const int BUCKET_SIZE = 8;
	// world size is 8x7
	static const int SIZE_X = 8;
	static const int SIZE_Y = 7;
	static const int ORIGIN_X = -2;
	static const int ORIGIN_Y = -2;
}
#endif
