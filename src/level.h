#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <memory>

class Level {
	private:
		int level_id = -1;
		int food_num = 0;
	public:
		Level();
		Level(int levelnum);
		void clearFood();
};

#endif
