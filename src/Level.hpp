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
#include "WorldConstants.hpp"

namespace Level
{
static const std::array<Fraction, World::SIZE_Y> ROWS = {{
	{1,1}, {5,4}, {25,16}, {125,64}, {729,343}, {81,49}, {9,7}
}};
static const std::array<Fraction, World::SIZE_X> COLS = {{
	{1,1}, {3,2}, {9,4}, {27,8}, {81,16}, {64,27}, {16,9}, {4,3}
}};

template<int size_x, int size_y>
static Fraction getJIAt(
	std::size_t x, std::size_t y,
	const std::array<Fraction, size_x> x_axis, 
	const std::array<Fraction, size_y> y_axis)
{
	Fraction x_ji = x_axis[x];
	Fraction y_ji = y_axis[y];
	return x_ji*y_ji;
}

template<int size_x, int size_y>
static std::array<std::array<Fraction, size_y>, size_x> genLattice(
	const std::array<Fraction, size_x> x_axis, 
	const std::array<Fraction, size_y> y_axis)
{
	std::array<std::array<Fraction, size_y>, size_x> lattice;
	for ( int i=0; i < size_x; ++i ) {
		for ( int j=0; j < size_y; ++j ) {
			lattice[i][j] = getJIAt<size_x,size_y>(i,j,x_axis,y_axis);
		}
	}
	return lattice;
}

static Fraction getJIAt(std::size_t x, std::size_t y)
{
	return getJIAt<World::SIZE_X,World::SIZE_Y>(x,y,COLS,ROWS);
}

static auto genLattice()
{
	return genLattice<World::SIZE_X, World::SIZE_Y>(COLS, ROWS);
}

void loadTest(entt::registry& reg, ManTex& man_tex,  ManAudio& man_audio);

void load(entt::registry& reg, ManTex& man_tex, std::string filename);
};

#endif
