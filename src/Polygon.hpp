#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <string>
#include <array>
#include <cmath>
#include <iostream>
#include <sstream>
#include <algorithm>

#include "CatConf.h"
#include "VectorMath.h"

class Polygon {
	private:
		std::vector<Vector2> vertices;
		//centre is the mean of all points
		Vector2 centre_pos; //RECALCULATE ON CHANGES TO VECTORFAN
		Vector2 bounds_min, bounds_max;	//RECALCULATE ON CHANGES TO VECTORFAN
		//recalculates the above two variables from scratch
		void recalculateData();

		void updateBound(const Vector2 new_pos);
		void recalculateBound();
		void recalculateBound(Vector2 removed_point);

		//gets the index of the vertex closest to the given position
		int getNearestIndex(Vector2 position) const;
	public:
		Polygon() {}
		Polygon(std::vector<Vector2> points);
		//this creates a circle with a resolution of pointnum
		Polygon(const Vector2 centre,
			const float radius,
			const int num_vertices);
		
		std::vector<Vector2> getVertices() const {return vertices;}
		int getNumVertices() const {return vertices.size();}
		Vector2 getCentre() const {return centre_pos;}
		Vector2 getVertexPos(int index) const;
		void moveVertex(int index, Vector2 new_pos);
		void addVertex(int index, Vector2 new_pos);
		void removeVertex(int index);
		//reverses the order of vertices
		void reverse();
		void clear();
		
		int getNearestClockwiseIndex(Vector2 pos) const;
		std::array<Vector2, 2> getNearestEdge(Vector2 pos);
		
		bool containsPoint(Vector2 point) const;
		//finds the percentage of b's vertices that are inside this vectorfan
		float findVertexCoverage(Polygon b) const;
		//finds the shape of the intersection with b
		std::vector<Vector2> findOverlapShape(Polygon b) const;

		std::vector<Vector2> getArrayScaled(float screen_scale) const;
		std::string printArray() const;
};

#endif
