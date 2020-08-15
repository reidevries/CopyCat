/*
 * texres.h
 *
 *  Created on: Mar 23, 2020
 *      Author: rei de vries
 *      This class represents an individual texture resource used by the game,
 *      and stores the time since it was last displayed on screen.
 */

#ifndef SRC_OLD_TEXRES_H_
#define SRC_OLD_TEXRES_H_

#include <raylib.h>
#include <iostream>

#include "../DebugPrinter.h"

class TexRes {
private:
	int tex_id;
	unsigned short last_seen_s = 0;	//in seconds since last seen on the screen
public:
	TexRes(int id);

	void age() {++last_seen_s;} //call every time one second passes
	void seen() {last_seen_s = 0;}
	unsigned short getAge() const {return last_seen_s;}
	~TexRes();
};

#endif /* SRC_OLD_TEXRES_H_ */
