#ifndef LEVEL_H
#define LEVEL_H

#include <array>
#include <memory>

#include <entt/entity/registry.hpp>
#include "Factory.hpp"
#include "ManTex.hpp"
#include "DebugPrinter.hpp"
#include "ManAudio.hpp"

namespace Level
{
//if a level bigger than this is generated, modulo op is used to find
//the position in the lattice
static const int SIZE_X = 8;
static const int SIZE_Y = 7;

template<int size_x, int size_y>
static std::array<std::array<float, size_y>, size_x> genLattice(
	const float x_axis[size_x], 
	const float y_axis[size_y])
{
	std::array<std::array<float, size_y>, size_x> lattice;
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
