#ifndef LEVEL_H
#define LEVEL_H

#include <array>
#include <memory>

#include <entt/entity/registry.hpp>
#include "Factory.hpp"
#include "ManTex.hpp"
#include "DebugPrinter.hpp"
#include "ManAudio.hpp"
#include "Fraction.hpp"

namespace Level
{
//if a level bigger than this is generated, modulo op is used to find
//the position in the lattice
static const int SIZE_X = 8;
static const int SIZE_Y = 7;
static const std::array<Fraction, SIZE_Y> rows {{
	{1,1}, {5,4}, {25,16}, {125,64}, {729,343}, {81,49}, {9,7}
}};
static const std::array<Fraction, SIZE_X> cols = {{
	{1,1}, {3,2}, {9,4}, {27,8}, {81,16}, {64,27}, {16,9}, {4,3}
}};

template<int size_x, int size_y>
static std::array<std::array<Fraction, size_y>, size_x> genLattice(
	const std::array<Fraction, size_x> x_axis, 
	const std::array<Fraction, size_y> y_axis)
{
	std::array<std::array<Fraction, size_y>, size_x> lattice;
	for ( int i=0; i < size_x; ++i ) {
		for ( int j=0; j < size_y; ++j ) {
			lattice[i][j] = x_axis[i]*y_axis[j];
		}
	}
	return lattice;
}

void loadTest(entt::registry& reg, ManTex& man_tex,  ManAudio& man_audio);

void load(entt::registry& reg, ManTex& man_tex, std::string filename);
};

#endif
