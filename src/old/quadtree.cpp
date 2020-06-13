/*
 * quadtree.cpp
 *
 *  Created on: Jun 10, 2020
 *      Author: rei de vries
 *  NOT USED OR TESTED!!!!!!!!!!!!!!!!!!!
 */

#include "quadtree.h"
#include "debugprinter.h"

using namespace std;

QuadTree::QuadTree(int depth, Vector2 field_size)
{
	this->depth = depth;
	this->field_size_half = {field_size.x/2, field_size.y/2};
	if (depth > 0) {
		for (int i = 0; i < 4; ++i) {
			children[i] = QuadTree(depth-1, field_size_half);
		}
	}
}

bool QuadTree::childExists(Quadrant q) {
	if (isLeaf()) {
		DebugPrinter::printDebug("QuadTree::childExists",
			"error; trying to get child of leaf node", 0);
		return false;
	}
	if (q < 0 || q > 4) {
		DebugPrinter::printDebug("QuadTree::childExists",
			q, " is an invalid index", 0);
		return false;
	}
	if (!static_cast<bool>(children[q])) {
		DebugPrinter::printDebug("QuadTree::childExists",
			quad_name[q] + " is null", 0);
		return false;
	}
	return true;
}

Quadrant QuadTree::getQuadrant(Vector2 pos)
{
	if (pos.x < field_size_half.x) {
		if (pos.y < field_size_half.y) {
			return LEFT;
		} else {
			return TOP;
		}
	} else {
		if (pos.y < field_size_half.y) {
			return BOTTOM;
		} else {
			return RIGHT;
		}
	}
}

Vector2 QuadTree::getPositionWithinQuadrant(Vector2 pos, Quadrant q)
{
	switch(q) {
	case TOP:
		return {pos.x, pos.y-field_size_half.y};
	case BOTTOM:
		return {pos.x-field_size_half.x, pos.y};
	case LEFT:
		return pos;
	case RIGHT:
		return {pos.x-field_size_half.x, pos.y-field_size_half.y};
	default:
		DebugPrinter::printDebug("QuadTree::getPositionWithinQuadrant",
			"error; No such quadrant index", 0);
		return {-1,-1};
	}
}

Vector2 QuadTree::getPositionWithinQuadrant(Vector2 pos)
{
	Quadrant q = getQuadrant(pos);
	return getPositionWithinQuadrant(pos, q);
}

bool QuadTree::hasValidChildren()
{
	if (isLeaf()) {
		for (int i = 0; i < 4; ++i) {
			if (children[i]) {
				DebugPrinter::printDebug("QuadTree::hasValidChildren",
					depth,
					"depth validation failed; leaf node has children", 0);
				return false;
			}
		}
	} else {
		if (data.size() > 0) {
			DebugPrinter::printDebug("QuadTree::hasValidChildren",
				depth, "depth validation failed; stem node contains data", 0);
			return false;
		}
		for (int i = 0; i < 4; ++i) {
			if (!children[i]) {
				stringstream ss;
				ss << "depth validation failed; " << quad_name[i]
					<< " is null";
				DebugPrinter::printDebug("QuadTree::hasValidChildren",
					depth, ss.str().c_str(), 0);
				return false;
			} else if (!children[i]->hasValidChildren()) return false;
		}
	}
	return true;
}

vector<int> QuadTree::find(int datum)
{
	vector<int> result;
	if (isLeaf()) {
		auto search = data.find(datum);
		if (search != data.end()) {
			result.push_back(datum);
		}
	} else {
		for (int i = 0; i < 4; ++i) {
			vector<int> child_result = children[i]->find(datum);
			if (child_result.size() > 0) {
				result = child_result;
				result.push_back(i);
				break;
			}
		}
	}
	return result;
}

set<int> QuadTree::at(vector<int> indices)
{
	if (isLeaf()) {
		return data;
	} else if (indices.empty()) {
		return getData();
	} else {
		int index = indices.front();
		if (index < 0 || index >= 4) {
			DebugPrinter::printDebug("QuadTree::at",
				index,
				"is an illegal index, stopping search", 0);
			return getData();
		}
		indices.erase(indices.begin());
		return children[index]->at(indices);
	}
}

bool QuadTree::erase(const int datum, vector<int> indices)
{
	if (isLeaf()) {
		auto search = data.find(datum);
		if (search != data.end()) {
			data.erase(search);
			return true;
		} else {
			return false;
		}
	} else if (indices.empty()) {
		return false;
	} else {
		int index = indices.front();
		if (index < 0 || index >= 4) {
			DebugPrinter::printDebug("QuadTree::erase",
				index,
				"is an illegal index, returning false", 0);
			return false;
		}
		indices.erase(indices.begin());
		return children[index]->erase(datum, indices);
	}
}

bool QuadTree::erase(const int datum)
{
	if (isLeaf()) {
		auto search = data.find(datum);
		if (search != data.end()) {
			data.erase(search);
			return true;
		} else {
			return false;
		}
	} else {
		for (int i = 0; i < 4; ++i) {
			if (children[i]->erase(datum)) return true;
		}
		return false;
	}
}

void QuadTree::insert(int datum, Vector2 pos)
{
	if (isLeaf()) {
		data.insert(datum);
	} else {
		Quadrant q = getQuadrant(pos);
		if (q >= 0 && q < 4) {
			children[q]->insert(datum, getPositionWithinQuadrant(pos, q));
		}
	}
}

void QuadTree::replace(int datum, Vector2 pos)
{
	erase(datum);
	insert(datum, pos);
}

void QuadTree::clear() {
	if (isLeaf()) {
		data.clear();
	} else {
		for (int i = 0; i < 4; ++i) {
			children[i]->clear();
		}
	}
}

set<int> QuadTree::getData() {
	if (isLeaf()) {
		return data;
	} else {
		set<int> aggregated_data;
		for (int i = 0; i < 4; ++i) {
			if (childExists(static_cast<Quadrant>(i))) {
				set<int> child_data = children[i]->getData();
				aggregated_data.insert(child_data.begin(), child_data.end());
			}
		}
	}
}

set<int> QuadTree::getData(Vector2 pos, int max_depth) {
	if (depth <= max_depth) {
		return getData();
	} else {
		Quadrant q = getQuadrant(pos);
		return children[q]->getData(getPositionWithinQuadrant(pos, q), max_depth);
	}
}

