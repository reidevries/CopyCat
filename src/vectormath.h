#ifndef VECTORMATH_H
#define VECTORMATH_H

#include <cmath>
#include <raylib.h>
#include <vector>
#include <string>
#include <sstream>

static std::string printVector(Vector2 a) {
	std::stringstream ss;
	ss << a.x << "," << a.y;
	return ss.str();
}

class VectorMath {
	public:
		static Vector2 add(Vector2 a, Vector2 b);
		static Vector2 sub(Vector2 a, Vector2 b);
		static Vector2 scale(Vector2 a, double b);
		static Vector2 scale(Vector2 a, Vector2 b);
		static bool checkPointInPolygon(Vector2 point, std::vector<Vector2> polygon);
		static Vector2 normalise(Vector2 a);
		static Vector2 intersectLines(Vector2 a_1, Vector2 a_2, Vector2 b_1, Vector2 b_2);	//returns a vector2 of t_a and t_b
		static Vector2 nearestPointOnSegment(Vector2 a_1, Vector2 a_2, Vector2 b);
		static float distanceToLine(Vector2 a_1, Vector2 a_2, Vector2 b);
		static float distanceToSegment(Vector2 a_1, Vector2 a_2, Vector2 b);
		static Color hsvToRgb(Color hsv);
		static Color hsvToRgb(unsigned char h, unsigned char s, unsigned char v, unsigned char a);
		static float dot(Vector2 a, Vector2 b);
		static float dist(Vector2 a, Vector2 b);
		static float sqrdist(Vector2 a, Vector2 b);
};

#endif
