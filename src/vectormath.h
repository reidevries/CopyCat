#ifndef VECTORMATH_H
#define VECTORMATH_H

#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <cstdint>

#include "catconf.h"

namespace VectorMath {
constexpr double DEG_TO_RAD = 0.01745329252;
constexpr double RAD_TO_DEG = 57.295779513;
constexpr Matrix identity = {
	1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

constexpr double degToRad(double deg)
{
	return deg*DEG_TO_RAD;
}

constexpr double radToDeg(double rad)
{
	return rad*RAD_TO_DEG;
}

static std::string printVector(const Vector2 a)
{
	return std::to_string(a.x) + "," + std::to_string(a.y);
}

static std::string printVector(const Vector3 a)
{
	return std::to_string(a.x)
		+ "," + std::to_string(a.y)
		+ "," + std::to_string(a.z);
}

static std::string printRect(const Rectangle a)
{
	return std::to_string(a.x) + ","
		+ std::to_string(a.y) + ","
		+ std::to_string(a.width) + "x"
		+ std::to_string(a.height);
}

constexpr double invSqrt(const double a)
{
#ifdef QUAKE
	//stolen from quake lol
	const float x2 = a * 0.5F;
	const float threehalfs = 1.5F;

	union {
		float f;
		uint32_t i;
	} conv = {dist2}; // member 'f' set to value of 'number'.
	conv.i  = 0x5f3759df - ( conv.i >> 1 );
	conv.f  *= ( threehalfs - ( x2 * conv.f * conv.f ) );
	return a*conv.f;
#else
	return 1.0/sqrt(a);
#endif
}

constexpr double dot(Vector2 a, Vector2 b)
{
	return a.x*b.x+a.y*b.y;
}

constexpr double dist(Vector2 a, Vector2 b)
{
	return sqrt( pow(b.x-a.x, 2) + pow(b.y-a.y, 2) );
}

constexpr Vector2 add(Vector2 a, Vector2 b)
{
	return (Vector2){a.x+b.x, a.y+b.y};
}

constexpr Vector2 addModulo(Vector2 a, Vector2 b, unsigned int m)
{
	Vector2 sum = {a.x+b.x, a.y+b.y};
	Vector2 decimal_part = {
		sum.x-static_cast<int>(sum.x),
		sum.y-static_cast<int>(sum.y)
	};
	return (Vector2) {
		decimal_part.x + (static_cast<int>(sum.x)%m),
		decimal_part.y + (static_cast<int>(sum.y)%m)
	};
}

constexpr Vector3 addModulo(Vector3 a, Vector3 b, unsigned int m)
{
	Vector3 sum = {a.x+b.x, a.y+b.y, a.z+b.z};
	Vector3 decimal_part = {
		sum.x-static_cast<int>(sum.x),
		sum.y-static_cast<int>(sum.y),
		sum.z-static_cast<int>(sum.z)
	};
	return (Vector3) {
		decimal_part.x + (static_cast<int>(sum.x)%m),
		decimal_part.y + (static_cast<int>(sum.y)%m),
		decimal_part.z + (static_cast<int>(sum.z)%m)
	};
}

constexpr Vector3 add(Vector3 a, Vector3 b)
{
	return (Vector3){a.x+b.x, a.y+b.y, a.z+b.z};
}

constexpr Vector2 sub(Vector2 a, Vector2 b)
{
	return (Vector2){a.x-b.x, a.y-b.y};
}

constexpr Vector3 sub(Vector3 a, Vector3 b)
{
	return (Vector3){a.x-b.x, a.y-b.y, a.z-b.z};
}

constexpr Vector2 scale(Vector2 a, float b)
{
	return (Vector2){a.x*b, a.y*b};
}

constexpr Vector3 scale(Vector3 a, float b)
{
	return (Vector3){a.x*b, a.y*b, a.z*b};
}

constexpr Vector2 scale(Vector2 a, Vector2 b)
{
	return (Vector2){a.x*b.x, a.y*b.y};
}

constexpr Vector3 scale(Vector3 a, Vector3 b)
{
	return (Vector3){a.x*b.x, a.y*b.y, a.z*b.z};
}

constexpr float max(Vector2 a)
{
	return (a.x>a.y)?a.x:a.y;
}

constexpr float max(Vector3 a)
{
	return (a.x>a.y)?
		((a.x>a.z)?a.x:a.z):((a.y>a.z)?a.y:a.z);
}

constexpr Vector2 dot(Vector2 a, Vector4 mat)
{
	return {
		a.x*mat.w + a.y*mat.x,
		a.x*mat.y + a.y*mat.z
	};
}

static bool checkPointInPolygon(const Vector2 point,
	std::vector<Vector2> polygon)
{
	int counter = 0;
	std::vector<Vector2>::iterator next_it;
	for (auto it = polygon.begin(); it != polygon.end(); it++) {
		next_it = std::next(it);
		if (next_it == polygon.end()) {
			next_it = polygon.begin();
		}

		Vector2 j = *it;
		Vector2 i = *next_it;
		//check if the test point y lies between the test vertices on the y axis
		if (((i.y > point.y) != (j.y > point.y)) &&
			//calculate the x value of the line at y=point.y, and check if the point is less than it
			( point.x < (j.x-i.x)*(point.y-i.y)/(j.y-i.y)+i.x)) {
			counter++;
		}
	}

	return (counter%2 == 1);
}

constexpr Vector2 normalise(Vector2 a)
{
	double dist = invSqrt(a.x*a.x+a.y*a.y);
	return VectorMath::scale(a, dist);
}

constexpr Vector3 normalise(Vector3 a)
{
	double dist = invSqrt(a.x*a.x+a.y*a.y+a.z*a.z);
	return VectorMath::scale(a,dist);
}

constexpr Vector3 normalize_shit_fast(Vector3 a)
{
	return VectorMath::scale(a, 1/max(a));
}

static Vector2 intersectLines(const Vector2 a_1,
	const Vector2 a_2,
	const Vector2 b_1,
	const Vector2 b_2)
{
	//starting from here http://cs.swan.ac.uk/~cssimon/line_intersection.html
	// rearranged it a bit so there were less factors,
	// as some factors were negatives of each other
	Vector2 aa_d = (Vector2){a_1.x-a_2.x, a_1.y-a_2.y};
	Vector2 ab_d = (Vector2){a_1.x-b_1.x, a_1.y-b_1.y};
	Vector2 bb_d = (Vector2){b_1.x-b_2.x, b_1.y-b_2.y};

	float denominator = aa_d.x*bb_d.y - bb_d.x*aa_d.y;
	//this case means the lines are coincident,
	// so it requires a different tactic to find if they intersect
	if (denominator == 0) {
		float t_x_1 = (a_1.x - b_1.x)/aa_d.x;
		float t_x_2 = (a_1.x - b_2.x)/aa_d.x;

		if (t_x_1 >= 0 && t_x_1 < 1) {
			//this case means b_1 is on the line segment between a_1 and a_2
			return b_1;
		} else if (t_x_2 >= 0 && t_x_2 < 1) {
			//this case means b_2 is on the line segment between a_1 and a_2
			return b_2;
		} else if (t_x_1 < 0 && t_x_2 >= 1) {
			//this case means the line segment b_1,b_2
			// encapsulates the line segment a_1,a_2
			return a_1;
		} else if (t_x_2 < 0 && t_x_1 >= 1) {
			//this case means the same thing,
			// but with b's slope being the inverse of a's
			return a_2;
		} else {
			//this case means that although the lines are coincident,
			// the line segments do not intersect
			return (Vector2){-1,-1};
		}
	} else {
		float t_a = (bb_d.y*ab_d.x - bb_d.x*ab_d.y)/denominator;
		float t_b = (aa_d.y*ab_d.x - aa_d.x*ab_d.y)/denominator;

		if (t_a >= 0 && t_b >= 0 && t_a < 1 && t_b < 1) {
			return VectorMath::add(a_1, VectorMath::scale(aa_d, -t_a));
		} else {
			return (Vector2){-1,-1};
		}
	}
}

static Vector2 nearestPointOnSegment(const Vector2 a_1,
	const Vector2 a_2,
	const Vector2 b)
{
	const Vector2 a_dir = VectorMath::normalise(VectorMath::sub(a_2, a_1));
	const Vector2 b_d = VectorMath::sub(b, a_1);

	float d = VectorMath::dot(a_dir, b_d);
	if (d < 0) d = 0;
	Vector2 nearest_point = VectorMath::add(a_1, VectorMath::scale(a_dir, d));
	if (VectorMath::dot(VectorMath::sub(b,a_2), a_dir) > 0) {
		//if the vector from a_2 to b points the same direction as the line,
		// b should be clamped to a_2
		return a_2;
	} else return nearest_point;
}

static double distanceToLine(const Vector2 a_1,
	const Vector2 a_2,
	const Vector2 b)
{
	//don't use this to find distance to a line segment,
	// use dist(nearestPointOnSegment()) instead
	Vector2 a_d = {a_2.x-a_1.x, a_2.y-a_1.y};
	float divisor = invSqrt(pow(a_d.y, 2) + pow(a_d.x, 2));
	return abs(a_d.y*b.x - a_d.x*b.y + a_2.x*a_1.y - a_2.y*a_1.x)*divisor;
}

static double distanceToSegment(const Vector2 a_1,
	const Vector2 a_2,
	const Vector2 b)
{
	return dist(nearestPointOnSegment(a_1,a_2,b), b);
}

static Color hsvToRgb(const Color hsv)
{
	Color rgb = WHITE;
	if (hsv.g == 0){
		rgb.r = hsv.b;
		rgb.g = hsv.b;
		rgb.b = hsv.b;
		rgb.a = hsv.a;
		return rgb;
	}

	const unsigned char region = hsv.r / 43;
	const unsigned char remainder = (hsv.r - (region * 43)) * 6;

	const unsigned char p = (hsv.b * (255 - hsv.g)) >> 8;
	const unsigned char q = (hsv.b * (255 - ((hsv.g * remainder) >> 8))) >> 8;
	const unsigned char t
		= (hsv.b * (255 - ((hsv.g * (255 - remainder)) >> 8))) >> 8;

	switch (region) {
		case 0:
			rgb.r = hsv.b;
			rgb.g = t;
			rgb.b = p;
			break;
		case 1:
			rgb.r = q;
			rgb.g = hsv.b;
			rgb.b = p;
			break;
		case 2:
			rgb.r = p;
			rgb.g = hsv.b;
			rgb.b = t;
            break;
        case 3:
            rgb.r = p;
            rgb.g = q;
            rgb.b = hsv.b;
            break;
        case 4:
            rgb.r = t;
            rgb.g = p;
            rgb.b = hsv.b;
            break;
        default:
            rgb.r = hsv.b;
            rgb.g = p;
            rgb.b = q;
            break;
    }

	rgb.a = hsv.a;

    return rgb;
}

static Color hsvToRgb(const unsigned char h,
	const unsigned char s,
	const unsigned char v,
	const unsigned char a)
{
	return hsvToRgb((Color){h,s,v,a});
}

static double sqrdist(const Vector2 a, const Vector2 b)
{
	return pow(b.x-a.x,2) + pow(b.y-a.y, 2);
}

static Vector2 convert3Dto2Dcoord(const Vector3 a,
	const Camera cam);	//converts a 3d coord to a screen coord
};

#endif
