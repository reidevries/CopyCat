/*
 * vectormath_test.cpp
 *
 *  Created on: Mar 15, 2020
 *      Author: rei de vries
 */

#include <iostream>
#include <raylib.h>
#include "src/vectormath.h"
#include "src/polygon.h"
#include <vector>
#include <memory>

int main(void) {
	Polygon test_pentagon((Vector2){5,5}, 3.5f, 5);
	std::vector<Vector2> trivertices = {(Vector2){8,2}, (Vector2){5,9}, (Vector2){2,2}};
	Polygon test_triangle(trivertices, 3);
	std::vector<Vector2> irregvertices = {(Vector2){6,1}, (Vector2){5,9}, (Vector2){3,8}, (Vector2){7,5}, (Vector2){2,2}};
	Polygon test_irregular(irregvertices, 3);

	std::cout << "testing checkPointInPolygon for regular pentagon" << std::endl;
	std::cout << "----0         1         2         3         4         5         6         7         8         9         10" << std::endl;
	for (int y = 0; y < 10; ++y) {
		std::cout << y << "---";
		for (int x = 0; x < 100; ++x) {
			if (VectorMath::checkPointInPolygon((Vector2){x/10.0f,y}, test_pentagon.getVertices())) {
				std::cout << "#";
			} else {
				std::cout << " ";
			}
		}
		std::cout << std::endl;
	}

	std::cout << "testing checkPointInPolygon for triangle" << std::endl;
	std::cout << "----0         1         2         3         4         5         6         7         8         9         10" << std::endl;
	for (int y = 0; y < 10; ++y) {
		std::cout << y << "---";
		for (int x = 0; x < 100; ++x) {
			if (VectorMath::checkPointInPolygon((Vector2){x/10.0f,y}, test_triangle.getVertices())) {
				std::cout << "#";
			} else {
				std::cout << " ";
			}
		}
		std::cout << std::endl;
	}

	std::cout << std::endl <<  "testing checkPointInPolygon for self-intersecting polygon" << std::endl;
	std::cout << "----0         1         2         3         4         5         6         7         8         9         10" << std::endl;
	for (int y = 0; y < 10; ++y) {
		std::cout << y << "---";
		for (int x = 0; x < 100; ++x) {
			if (VectorMath::checkPointInPolygon((Vector2){x/10.0f,y}, test_irregular.getVertices())) {
				std::cout << "#";
			} else {
				std::cout << " ";
			}
		}
		std::cout << std::endl;
	}

	const Vector2 topleft		= { 0, 0};
	const Vector2 left			= { 0, 5};
	const Vector2 bottomleft	= { 0,10};
	const Vector2 topright		= {10, 0};
	const Vector2 right			= {10, 5};
	const Vector2 bottomright	= {10,10};
	const Vector2 centre		= { 5, 5};
	const Vector2 top			= { 5, 0};
	const Vector2 bottom		= { 5, 10};

	std::cout << std::endl << "testing distanceToLine for horizontal line (=5)" << std::endl;
	std::cout << VectorMath::distanceToLine(topleft, topright, centre) << std::endl;
	std::cout << "testing distanceToLine for vertical line (=5)" << std::endl;
	std::cout << VectorMath::distanceToLine(topleft, bottomleft, centre) << std::endl;
	std::cout << "testing distanceToLine for intersecting diagonal line (=0)" << std::endl;
	std::cout << VectorMath::distanceToLine(topleft, bottomright, centre) << std::endl;
	std::cout << "testing distanceToLine for non-intersecting diagonal line (=7.07107)" << std::endl;
	std::cout << VectorMath::distanceToLine(topleft, bottomright, topright) << std::endl;
	std::cout << "testing distanceToLine for point that intersects outside the segment (=0)" << std::endl;
	std::cout << VectorMath::distanceToLine(centre, bottomright, topleft) << std::endl;
	std::cout << "testing distanceToLine for point that is nearest to a part of line outside the segment" << std::endl;
	std::cout << VectorMath::distanceToLine(centre, bottomright, top) << std::endl;

	std::cout << std::endl << "testing nearestPointOnLine for horizontal line (=5,0)" << std::endl;
	std::cout << printVector(VectorMath::nearestPointOnSegment(topleft, topright, centre)) << std::endl;
	std::cout << "testing nearestPointOnLine for vertical line (=0,5)" << std::endl;
	std::cout << printVector(VectorMath::nearestPointOnSegment(topleft, bottomleft, centre)) << std::endl;
	std::cout << "testing nearestPointOnLine for intersecting diagonal line (=5,5)" << std::endl;
	std::cout << printVector(VectorMath::nearestPointOnSegment(topleft, bottomright, centre)) << std::endl;
	std::cout << "testing nearestPointOnLine for non-intersecting diagonal line (=5,5)" << std::endl;
	std::cout << printVector(VectorMath::nearestPointOnSegment(topleft, bottomright, topright)) << std::endl;
	std::cout << "testing nearestPointOnLine for point that intersects outside the segment (=5,5)" << std::endl;
	std::cout << printVector(VectorMath::nearestPointOnSegment(centre, bottomright, topleft)) << std::endl;
	std::cout << "testing nearestPointOnLine for point that is nearest to a part of the line outside the segment (=5,5)" << std::endl;
	std::cout << printVector(VectorMath::nearestPointOnSegment(centre, bottomright, top)) << std::endl;

	std::cout << std::endl << "testing getNearestEdge on triangle, nearest to top" << std::endl;
	std::array<Vector2, 2> edge = test_triangle.getNearestEdge(top);
	std::cout << printVector(edge[0]) << " to " << printVector(edge[1]) << std::endl;
	std::cout << "testing nearest to centre left" << std::endl;
	edge = test_triangle.getNearestEdge(left);
	std::cout << printVector(edge[0]) << " to " << printVector(edge[1]) << std::endl;
	std::cout << "testing nearest to centre right" << std::endl;
	edge = test_triangle.getNearestEdge(right);
	std::cout << printVector(edge[0]) << " to " << printVector(edge[1]) << std::endl;
	std::cout << "testing nearest to bottom left" << std::endl;
	edge = test_triangle.getNearestEdge(bottomleft);
	std::cout << printVector(edge[0]) << " to " << printVector(edge[1]) << std::endl;
	std::cout << "testing nearest to bottom right" << std::endl;
	edge = test_triangle.getNearestEdge(bottomright);
	std::cout << printVector(edge[0]) << " to " << printVector(edge[1]) << std::endl;
}


