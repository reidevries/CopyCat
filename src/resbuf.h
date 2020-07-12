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
#include <sstream>
#include <iostream>
#include <cstdint>

template <class T, std::size_t buf_size>
class ResBuf {
private:
	std::array<T, buf_size> buf;
	//buf_free represents the age of the element
	//any integer >= MAX_BUF_SIZE means it's free
	//increments by 1 every time a new elem is inserted
	//if none are free, report error
	std::array<std::size_t, buf_size> buf_free;
	std::array<std::string, buf_size> buf_names;
	std::map<std::string, std::size_t> buf_by_name;

	//not the capacity of the buf, but the number of nonfree elements
	std::size_t entry_counter = 0;

	//constant time
	void freeEntry(std::size_t i)
	{
		buf_free[i] = buf_size;
		buf_by_name.erase(buf_names[i]);
		buf_names[i] = "empty";
		--entry_counter;
	}

	//not quite constant time, has to increment age
	void insert(std::size_t i, std::string name, T entry)
	{
		for (int i = 0; i < buf_size; ++i) ++buf_free[i];

		buf_free[i] = 0;
		buf_by_name.insert({name, i});
		buf_names[i] = name;
		buf[i] = entry;
		++entry_counter;
	}

public:
	ResBuf()
	{
		buf_free.fill(buf_size);
		buf_names.fill("empty");
		entry_counter = 0;
	}

	ResBuf(T default_object) : ResBuf()
	{
		buf.fill(default_object);
	}

	//return number of nonfree elements in the buf
	unsigned int count() const
	{
		return entry_counter;
	}

	unsigned int getBufSize() const
	{
		return buf_size;
	}

	unsigned int getFreeIndex() const
	{
		for (int i = 0; i < buf_size; ++i) {
			if (buf_free[i] >= buf_size) {
				return i;
			}
		}
		return buf_size;
	}

	unsigned int getOldestNonFree() const
	{
		unsigned int oldest_index = 0;
		unsigned int oldest_age = 0;
		for (int i = 0; i < buf_size; ++i) {
			//only find oldest age for non-free indices
			if (buf_free[i] > oldest_age
				&& buf_free[i] < buf_size) {
				oldest_age = buf_free[i];
				oldest_index = i;
			}
		}
		return oldest_index;
	}

	//figure out whether index of buf is free or not
	bool isFree(unsigned int index) const
	{
		if (index >= buf_size) return false;
		if (buf_free[index] < buf_size) return false;
		return true;
	}

	//constant time access, by copying rather than reference
	T get(std::size_t index) const
	{
		return buf[index];
	}

	//constant time access
	T& at(std::size_t index)
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
	std::string nameAt(std::size_t index) const
	{
		if (index < buf_size) {
			return buf_names[index];
		} else return std::string("NOT FOUND");
	}

	std::map<std::string, std::size_t> getBufMap() const
	{
		return buf_by_name;
	}

	//returns chosen index on success, MAX_BUF_SIZE on fail
	unsigned int push(std::string name, T entry)
	{
		unsigned int free_index = getFreeIndex();
		if (free_index < buf_size) {
			insert(free_index, name, entry);
		}
		return free_index;
	}

	//if "name" exists in the buffer, return it,
	//otherwise push new entry into the buffer
	unsigned int findOrPush(std::string name, T entry)
	{
		if (has(name)) {
			return find(name);
		} else {
			return push(name, entry);
		}
	}

	//only do this if u know what ur doin
	void replace(std::size_t index, T entry)
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
	T& pop(std::size_t index)
	{
		if (index > buf_size) {
			std::cout << "ResBuf error: " << index
				<< " out of range, max size is " << buf_size
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

	std::array<T, buf_size>& popAll()
	{
		for (int i = 0; i < buf_size; ++i) {
			freeEntry(i);
		}
		return buf;
	}

	void clear()
	{
		for (int i = 0; i < buf_size; ++i) {
			freeEntry(i);
		}
	}

	//for debugging
	std::string printBuf() {
		std::stringstream output;
		output << "index:\tvalue:\tage:\tname:\t" << std::endl;
		for (int i = 0; i < buf_size; ++i) {
			output << i << "\t"
				<< buf[i] << "\t"
				<< static_cast<int>(buf_free[i]) << "\t"
				<< buf_names[i] << "\t" << std::endl;
		}
		return output.str();
	}
};


#endif /* SRC_RESBUF_H_ */
