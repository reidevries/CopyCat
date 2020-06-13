/*
 * quadtree.h
 *
 *  Created on: Jun 10, 2020
 *      Author: rei de vries
 *  NOT USED OR TESTED!!!!!!!!!!!!!!!!!!!
 */

#ifndef SRC_QUADTREE_H_
#define SRC_QUADTREE_H_

#include <string>
#include <memory>
#include <vector>
#include <set>
#include <raylib.h>

//top=topleft, bottom=bottomright, left = bottomleft, right = topright
//did it this way because it's an isometric game so the top is actually
//the top left in world coords
enum Quadrant{ TOP = 0, BOTTOM = 1, LEFT = 2, RIGHT = 3};
std::string quad_name[4] = {"TOP", "BOTTOM", "LEFT", "RIGHT"};

class QuadTree
{
private:
	int depth;
	std::unique_ptr<QuadTree> children[4];
	std::set<int> data; //the list of ids in this quadtree node
	Vector2 field_size_half;

	//checks if child exists at index q
	bool childExists(Quadrant q);

	//check if this quadtree is a leaf
	bool isLeaf() {
		return (depth == 0);
	}

	//method for figuring out quadrants and positions within them
	Quadrant getQuadrant(Vector2 pos);
	Vector2 getPositionWithinQuadrant(Vector2 pos, Quadrant q);
	Vector2 getPositionWithinQuadrant(Vector2 pos);

public:
	QuadTree(int max_depth, Vector2 field_size);

	//validates whether this node's children are structured properly
	//if they're not, it prints an error
	bool hasValidChildren();

	//tries to find the datum in the quadtree
	//if it is found, returns the path to the object down the tree
	//otherwise, returns an empty vector
	std::vector<int> find(int datum);
	//returns the data down a particular line of indices
	std::set<int> at(std::vector<int> indices);
	//erases a particular datum down a particular line of indices
	//returns true on success, false if the datum isn't found
	bool erase(int datum, std::vector<int> indices);
	bool erase(int datum);
	void insert(int datum, Vector2 pos);
	void replace(int datum, Vector2 pos);
	std::set<int> getData();
	std::set<int> getData(Vector2 pos, int max_depth);
	void clear();
};



#endif /* SRC_QUADTREE_H_ */
