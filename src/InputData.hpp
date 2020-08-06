/*
 * Created: 02/08 2020
 *  by Rei de Vries
 */

#ifndef INPUTDATA_HPP_
#define INPUTDATA_HPP_

#include "CatConf.hpp"

class InputData
{
public:
	struct Mouse {
		Ray ray;
		Vector2 screen_pos;
		struct Button {
			bool down;
			bool press;
			bool rls;
		};
		Button l;
		Button m;
		Button r;
		int scroll;
	};
	
private:
	Mouse mouse;

public:
	// needs Camera for mouse raycast
	void updateValues(const Camera cam);
	const Mouse& getMouseData() const {return mouse;}
};

#endif // INPUTDATA_H
