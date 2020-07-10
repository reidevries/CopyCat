/*
 * resbuf.h
 *
 *  Created on: Jul 1, 2020
 *      Author: rei de vries
 *	Provides constant-time member access by ID,
 *	or logn time member access by a name string
 *	raises an error if it runs out of space
 */

#ifndef SRC_RESBUF_H_
#define SRC_RESBUF_H_

#include <array>
#include <map>
#include <string>
#include <iostream>
#include <cstdint>

namespace Res {
	static const uint8_t MAX_BUF_SIZE = 255;
}

template <class T>
class ResBuf {
private:
	std::array<T, Res::MAX_BUF_SIZE> buf;
	//buf_free represents the age of the element
	//any integer >= MAX_BUF_SIZE means it's free
	//increments by 1 every time a new elem is inserted
	//if none are free, report error
	std::array<uint8_t, Res::MAX_BUF_SIZE> buf_free;
	std::array<std::string, Res::MAX_BUF_SIZE> buf_names;
	std::map<std::string, uint8_t> buf_by_name;

	//not the capacity of the buf, but the number of nonfree elements
	uint8_t entry_counter = 0;

	//constant time
	void freeEntry(uint8_t i)
	{
		buf_free[i] = Res::MAX_BUF_SIZE;
		buf_by_name.erase(buf_names[i]);
		buf_names[i] = "";
		--entry_counter;
	}

	//not quite constant time, has to increment age
	void insert(uint8_t i, std::string name, T entry)
	{
		std::cout << "inserted " << name << " at index " << i << std::endl;
		for (int i = 0; i < Res::MAX_BUF_SIZE; ++i) ++buf_free[i];

		buf_free[i] = 0;
		buf_by_name.insert({name, i});
		buf_names[i] = name;
		buf[i] = entry;
		++entry_counter;
	}
public:
	ResBuf()
	{
		//set every entry in buf_free to MAX_BUF_SIZE
		//to indicate they're all currently free
		for (int i = 0; i < Res::MAX_BUF_SIZE; ++i) {
			buf_free[i] = Res::MAX_BUF_SIZE;
		}
	}

	//return number of nonfree elements in the buf
	unsigned int count() const
	{
		return entry_counter;
	}

	unsigned int getFreeIndex() const
	{
		for (int i = 0; i < Res::MAX_BUF_SIZE; ++i) {
			if (buf_free[i] >= Res::MAX_BUF_SIZE) {
				return i;
			}
		}
		return Res::MAX_BUF_SIZE;
	}

	unsigned int getOldestNonFree() const
	{
		unsigned int oldest_index = 0;
		unsigned int oldest_age = 0;
		for (int i = 0; i < Res::MAX_BUF_SIZE; ++i) {
			//only find oldest age for non-free indices
			if (buf_free[i] > oldest_age
				&& buf_free[i] < Res::MAX_BUF_SIZE) {
				oldest_age = buf_free[i];
				oldest_index = i;
			}
		}
		return oldest_index;
	}

	//figure out whether index of buf is free or not
	bool isFree(unsigned int index) const
	{
		if (index >= Res::MAX_BUF_SIZE) return false;
		if (buf_free[index] < Res::MAX_BUF_SIZE) return false;
		return true;
	}

	//constant time access, by copying rather than reference
	T get(uint8_t index) const
	{
		return buf[index];
	}

	//constant time access
	T& at(uint8_t index)
	{
		return buf[index];
	}

	//logn time access
	T& at(std::string name)
	{
		return buf[buf_by_name.at(name)];
	}

	//logn time
	int find(std::string name) const
	{
		return buf_by_name.at(name);
	}

	//figure out whether buf has a certain name in logn time
	bool has(std::string name) const
	{
		return (buf_by_name.count(name) > 0);
	}

	//gets name at index
	std::string nameAt(uint8_t index) const
	{
		if (index < Res::MAX_BUF_SIZE) {
			return buf_names[index];
		} else return std::string("NOT FOUND");
	}

	std::map<std::string, uint8_t> getBufMap() const
	{
		return buf_by_name;
	}

	//returns chosen index on success, MAX_BUF_SIZE on fail
	unsigned int push(std::string name, T entry)
	{
		unsigned int free_index = getFreeIndex();
		if (free_index < Res::MAX_BUF_SIZE) {
			insert(free_index, name, entry);
		}
		return free_index;
	}

	//only do this if u know what ur doin
	void replace(uint8_t index, T entry)
	{
		if (isFree(index)) {
			std::cout << "ResBuf::replace error: " << index
				<< " is free, but trying to replace entry. "
				<< " U obviously didn't read the comment in resbuf.h"
				<< std::endl;
			return;
		}
		buf.at(index) = entry;
	}

	//fetches a reference to the element at the given index
	//and sets it to be free.
	//MUST unload referenced object externally to avoid leaks
	//should be constant time
	T& pop(uint8_t index)
	{
		if (index > Res::MAX_BUF_SIZE) {
			std::cout << "ResBuf error: " << index
				<< " out of range, max size is " << Res::MAX_BUF_SIZE
				<< std::endl;
			std::cout << "Returning first element in array";
			return buf[0];
		}

		if (isFree(index)) {
			std::cout << "ResBuf error: "
				<< index << " already free"
				<< std::endl;
			std::cout << "Returning first element in array";
			return buf[0];
		}

		freeEntry(index);
		return buf[index];
	}

	//run pop(getOldestNonFree())
	T& popOldest() {
		int oldest_index = getOldestNonFree();
		freeEntry(oldest_index);
		return buf[oldest_index];
	}

	std::array<T, Res::MAX_BUF_SIZE>& popAll()
	{
		for (int i = 0; i < Res::MAX_BUF_SIZE; ++i) {
			freeEntry(i);
		}
		return buf;
	}
};


#endif /* SRC_RESBUF_H_ */
