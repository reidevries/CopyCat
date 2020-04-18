#ifndef VECTORFAN_H
#define VECTORFAN_H

#include <raylib.h>
#include <vector>
#include <string>
#include <array>

class Polygon {
	private:
		std::vector<Vector2> vertices;
		Vector2 centre_pos;										//RECALCULATE ON CHANGES TO VECTORFAN stores the centre position (centre = average of all points)
		Vector2 bounds_min, bounds_max;							//RECALCULATE ON CHANGES TO VECTORFAN stores the bounding box of the vectorfan
		void recalculateData();									//recalculates the above two variables from scratch, which is the least efficient way to do it
		void updateBound(Vector2 newpos);
		void recalculateBound();
		void recalculateBound(Vector2 removed_point);
		int getNearestIndex(Vector2 position);					//gets the index of the point closest to the given position
	public:
		Polygon();
		Polygon(std::vector<Vector2> points, int point_num);
		Polygon(Vector2 centre, float radius, int point_num);	//this creates a circle with a resolution of pointnum
		
		std::vector<Vector2> getVertices() {return vertices;}
		int getPointNum() {return vertices.size();}
		Vector2 getCentre() {return centre_pos;}				//gets the "centre"
		Vector2 getPointPos(int index);							//gets the position of the point at index
		void movePoint(int index, Vector2 newpos);				//moves a point to newpos, and updates the centrepos
		void addPoint(int index, Vector2 newpoint);				//adds a point, and updates the centrepos
		void removePoint(int index);							//removes a poin, and updates the centrepos
		void reverse();											//reverses the entire fan
		void clear();											//removes all points
		
		int getNearestClockwiseIndex(Vector2 pos);
		std::array<Vector2, 2> getNearestEdge(Vector2 pos);
		
		bool containsPoint(Vector2 point);
		float findVertexCoverage(Polygon b);					//finds the percentage of b's vertices that are inside this vectorfan
		std::vector<Vector2> findOverlapShape(Polygon b); 	//finds the shape of the overlap between this vectorfan and the given one

		Vector2* getArrayScaled(float screen_scale);
		std::string printArray();
};

#endif
