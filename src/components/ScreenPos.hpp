/*
 * ScreenPos.h
 *
 *  Created on: Jul 17, 2020
 *      Author: rei de vries
 *      component for screen position in 2D
 *      position is specified as a fraction of screen width/height
 */

#ifndef SRC_COMPONENTS_SCREENPOS_H_
#define SRC_COMPONENTS_SCREENPOS_H_


struct ScreenPos
{
	Vector2 pos;

	ScreenPos() : pos({0,0}) {}
	ScreenPos(Vector2 set_pos) : pos(set_pos) {}
};



#endif /* SRC_COMPONENTS_SCREENPOS_H_ */
