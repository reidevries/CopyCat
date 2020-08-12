/*
 * RaylibOps.h
 *
 *  Created on: July 30, 2020
 *      Author: rei de vries
 *      This overloads operators for raylib structs
 */

#ifndef SRC_RAYLIBOPS_H_
#define SRC_RAYLIBOPS_H_

#include "CatConf.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>










// Vector2 Overloads----------------------------------------------------------

constexpr Vector2 operator+( const Vector2& v1, const Vector2& v2 )
{
	return {
		v1.x+v2.x,
		v1.y+v2.y
	};
}

constexpr Vector2 operator-( const Vector2& v1, const Vector2& v2 )
{
	return {
		v1.x-v2.x,
		v1.y-v2.y
	};
}

constexpr Vector2 operator*( const Vector2& v, const float& f )
{
	return {
		v.x*f,
		v.y*f
	};
}

constexpr Vector2 operator*( const float& f, const Vector2& v )
{
	return {
		v.x*f,
		v.y*f
	};
}

constexpr Vector2 operator% ( const Vector2& v, const int& m )
{
	int floor_x = static_cast<int>(v.x); 
	int floor_y = static_cast<int>(v.y);
	return {
		(v.x - floor_x) + (floor_x%m),
		(v.y - floor_y) + (floor_y%m)
	};
}

constexpr Vector2 operator% ( const Vector2& v, const Vector2& m )
{
	int floor_x = static_cast<int>(v.x); 
	int floor_y = static_cast<int>(v.y);
	return {
		(v.x - floor_x) + (floor_x%static_cast<int>(m.x)),
		(v.y - floor_y) + (floor_y%static_cast<int>(m.y))
	};
}

// this does dot product
constexpr float operator*( const Vector2& v1, const Vector2& v2 )
{
	return v1.x*v2.x+v1.y*v2.y;
}

// also does dot product,  but with vector4 treated as 2x2 matrix
constexpr Vector2 operator*( const Vector2& v, const Vector4& mat )
{
	return {
		v.x*mat.w + v.y*mat.x,
		v.x*mat.y + v.y*mat.z
	};
}

// this does scalar product
constexpr Vector2 operator^( const Vector2& v1, const Vector2& v2 )
{
	return {
		v1.x*v2.x,
		v1.y*v2.y
	};
}

constexpr Vector2 operator/( const float& f,  const Vector2& v )
{
	return {
		v.x/f, 
		v.y/f
	};
}

constexpr Vector2 operator/( const Vector2& v1,  const Vector2& v2 )
{
	return {
		v1.x/v2.x, 
		v1.y/v2.y
	};
}

constexpr bool operator==(const Vector2& v1, const Vector2& v2 )
{
	return (v1.x==v2.x)&&(v1.y==v2.y);
}

constexpr bool operator!=(const Vector2& v1, const Vector2& v2 )
{
	return !(v1==v2);
}

inline
std::ostream& operator<<( std::ostream& output, const Vector2& v )
{
	output << v.x << ", " << v.y;
	return output;
}

inline
std::istream& operator>>( std::istream& input, Vector2& v )
{
	input >> v.x >> v.y;
	return input;
}

//----------------------------------------------------------------------------










// Vector3 Overloads----------------------------------------------------------

constexpr Vector3 operator+( const Vector3& v1, const Vector3& v2 )
{
	return {
		v1.x+v2.x,
		v1.y+v2.y,
		v1.z+v2.z
	};
}

constexpr Vector3 operator-( const Vector3& v1, const Vector3& v2 )
{
	return {
		v1.x-v2.x,
		v1.y-v2.y,
		v1.z-v2.z
	};
}

constexpr Vector3 operator*( const Vector3& v, const float& f )
{
	return {
		v.x*f,
		v.y*f,
		v.z*f
	};
}

constexpr Vector3 operator*( const float& f, const Vector3& v )
{
	return {
		v.x*f,
		v.y*f,
		v.z*f
	};
}

constexpr Vector3 operator% ( const Vector3& v, const int& m )
{
	int floor_x = static_cast<int>(v.x); 
	int floor_y = static_cast<int>(v.y);
	int floor_z = static_cast<int>(v.z);
	return {
		(v.x - floor_x) + (floor_x%m),
		(v.y - floor_y) + (floor_y%m), 
		(v.z - floor_z) + (floor_z%m) 
	};
}

constexpr Vector3 operator% ( const Vector3& v, const Vector3& m )
{
	int floor_x = static_cast<int>(v.x); 
	int floor_y = static_cast<int>(v.y);
	int floor_z = static_cast<int>(v.z);
	return {
		(v.x - floor_x) + (floor_x%static_cast<int>(m.x)),
		(v.y - floor_y) + (floor_y%static_cast<int>(m.y)), 
		(v.z - floor_z) + (floor_z%static_cast<int>(m.z))
	};
}

// this does dot product
constexpr float operator*( const Vector3& v1, const Vector3& v2 )
{
	return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z;
}

// this does scalar product
constexpr Vector3 operator^( const Vector3& v1, const Vector3& v2 )
{
	return {
		v1.x*v2.x,
		v1.y*v2.y,
		v1.z*v2.z
	};
}

constexpr Vector3 operator/( const float& f,  const Vector3& v )
{
	return {
		v.x/f, 
		v.y/f, 
		v.z/f
	};
}

constexpr Vector3 operator/( const Vector3& v1,  const Vector3& v2 )
{
	return {
		v1.x/v2.x, 
		v1.y/v2.y, 
		v1.z/v2.z
	};
}

constexpr bool operator==(const Vector3& v1, const Vector3& v2 )
{
	return (v1.x==v2.x)&&(v1.y==v2.y)&&(v1.z==v2.z);
}

inline
std::ostream& operator<<( std::ostream& output, const Vector3& v )
{
	output << v.x << ", " << v.y << ", " << v.z;
	return output;
}

inline
std::istream& operator>>( std::istream& input, Vector3& v )
{
	input >> v.x >> v.y >> v.z;
	return input;
}

//----------------------------------------------------------------------------










// Vector4 Overloads----------------------------------------------------------

constexpr Vector4 operator+( const Vector4& v1, const Vector4& v2 )
{
	return {
		v1.x+v2.x,
		v1.y+v2.y,
		v1.z+v2.z, 
		v1.w+v2.w
	};
}

constexpr Vector4 operator-( const Vector4& v1, const Vector4& v2 )
{
	return {
		v1.x-v2.x,
		v1.y-v2.y,
		v1.z-v2.z, 
		v1.w-v2.w
	};
}

constexpr Vector4 operator*( const Vector4& v, const float& f )
{
	return {
		v.x*f,
		v.y*f,
		v.z*f, 
		v.w*f
	};
}

constexpr Vector4 operator*( const float& f, const Vector4& v )
{
	return {
		v.x*f,
		v.y*f,
		v.z*f, 
		v.w*f
	};
}

constexpr Vector4 operator% ( const Vector4& v, const int& m )
{
	int floor_x = static_cast<int>(v.x); 
	int floor_y = static_cast<int>(v.y);
	int floor_z = static_cast<int>(v.z);
	int floor_w = static_cast<int>(v.w);
	return {
		(v.x - floor_x) + (floor_x%m),
		(v.y - floor_y) + (floor_y%m), 
		(v.z - floor_z) + (floor_z%m), 
		(v.w - floor_w) + (floor_w%m) 
	};
}

constexpr Vector4 operator% ( const Vector4& v, const Vector4& m )
{
	int floor_x = static_cast<int>(v.x); 
	int floor_y = static_cast<int>(v.y);
	int floor_z = static_cast<int>(v.z);
	int floor_w = static_cast<int>(v.w);
	return {
		(v.x - floor_x) + (floor_x%static_cast<int>(m.x)),
		(v.y - floor_y) + (floor_y%static_cast<int>(m.y)), 
		(v.z - floor_z) + (floor_z%static_cast<int>(m.z)), 
		(v.z - floor_w) + (floor_z%static_cast<int>(m.w))
	};
}

// this does dot product
constexpr float operator*( const Vector4& v1, const Vector4& v2 )
{
	return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z+v1.w*v2.w;
}

// dot product between vector4 and 4x4 matrix
constexpr Vector4 operator*( const Vector4& v,  const Matrix& m )
{
	return {
		v.x*m.m0+v.y*m.m1+v.z*m.m2+v.w*m.m3, 
		v.x*m.m4+v.y*m.m5+v.z*m.m6+v.w*m.m7, 
		v.x*m.m8+v.y*m.m9+v.z*m.m10+v.w*m.m11, 
		v.x*m.m12+v.y*m.m13+v.z*m.m14+v.w*m.m15
	};
}

// this does scalar product
constexpr Vector4 operator^( const Vector4& v1, const Vector4& v2 )
{
	return {
		v1.x*v2.x,
		v1.y*v2.y,
		v1.z*v2.z, 
		v1.w*v2.w
	};
}

constexpr Vector4 operator/( const float& f,  const Vector4& v )
{
	return {
		v.x/f, 
		v.y/f, 
		v.z/f, 
		v.w/f
	};
}

constexpr Vector4 operator/( const Vector4& v1,  const Vector4& v2 )
{
	return {
		v1.x/v2.x, 
		v1.y/v2.y, 
		v1.z/v2.z, 
		v1.w/v2.w
	};
}

constexpr bool operator==(const Vector4& v1, const Vector4& v2 )
{
	return (v1.x==v2.x)&&(v1.y==v2.y)&&(v1.z==v2.z)&&(v1.w==v2.w);
}

inline
std::ostream& operator<<( std::ostream& output, const Vector4& v )
{
	output << v.x << ", " << v.y << ", " << v.z << ", " << v.w;
	return output;
}

inline
std::istream& operator>>( std::istream& input, Vector4& v )
{
	input >> v.x >> v.y >> v.z >> v.w;
	return input;
}

//----------------------------------------------------------------------------










// Rect Overloads-------------------------------------------------------------

inline
std::ostream& operator<<( std::ostream& output, const Rectangle& v )
{
	output << v.x << ", " << v.y << " - " << v.width << "x" << v.height;
	return output;
}

inline
std::istream& operator>>( std::istream& input, Rectangle& v )
{
	input >> v.x >> v.y >> v.width >> v.height;
	return input;
}

//----------------------------------------------------------------------------









// Color Overloads------------------------------------------------------------

// stream operators use hex code format, only print alpha if transparent
inline
std::ostream& operator<<( std::ostream& output, const Color& c )
{
	output << "#" << std::setw(2) << std::hex << static_cast<int>(c.r)
		<< std::setw(2) << std::hex << static_cast<int>(c.g)
		<< std::setw(2) << std::hex << static_cast<int>(c.b);
	if (c.a < 255) {
		output << std::setw(2) << std::hex << static_cast<int>(c.a); 
	}
	return output;
}

inline
std::istream& operator>>( std::istream& input, Color& c )
{
	std::string s;
	input >> s;
	
	if (s.front() == '#') {
		c.r = std::stoul(s.substr(1, 2), nullptr, 16);
		c.g = std::stoul(s.substr(3, 2), nullptr, 16);
		c.b = std::stoul(s.substr(5, 2), nullptr, 16);
		if (s.size() > 7) {
			c.a = std::stoul(s.substr(7, 2), nullptr, 16);
		}
	} else {
		std::stringstream ss(s);
		ss >> c.r >> c.g >> c.b >> c.a;
	}
	
	return input;
}

//----------------------------------------------------------------------------


// 
#endif
