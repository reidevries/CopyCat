/*
 * resbuf.h
 *
 *  Created on: Jul 1, 2020
 *      Author: rei de vries
 *	Provides constant-time member access by ID,
 *	or logn time member access by a name string
 *	raises an error if it runs out of space.
 *
 *	data is stored on the stack, the container is deliberately fixed size
 *	data is never deleted in this class, u have to do that urself
 *	or use the "clear" methods
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
	//buf_free represents whether the element is valid
	//the element will still exist in memory, but not be used
	std::array<bool, buf_size> buf_free;
	std::array<std::string, buf_size> buf_names;
	std::map<std::string, std::size_t> buf_by_name;

	//not the capacity of the buf, but the number of nonfree elements
	std::size_t entry_counter = 0;

	//round robin counter to keep track of oldest elements
	std::size_t rr = 0;

	void incRR()
	{
		rr = (rr+1)%buf_size;
	}

	//constant time
	void freeEntry(std::size_t i)
	{
		buf_free[i] = true;
		buf_by_name.erase(buf_names[i]);
		buf_names[i] = "empty";
		--entry_counter;
	}

	//constant time, inserts into a specific index
	void insert(std::size_t i, std::string name, T entry)
	{
		buf_free[i] = false;
		buf_by_name.insert({name, i});
		buf_names[i] = name;
		buf[i] = entry;
		++entry_counter;
	}

	//constant time, inserts based on round robin counter
	std::size_t insert(std::string name, T entry)
	{
		std::size_t index = getFreeIndex();
		insert(index, name, entry);
		incRR();
		return index;
	}

	std::size_t forceInsert(std::string name, T entry)
	{
		std::size_t index = rr;
		insert(index, name, entry);
		incRR();
		return index;
	}

	//for printing debug output
	std::string printBufFree(std::size_t i)
	{
		return (buf_free[i])?std::string("true"):std::string("false");
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
	std::size_t count() const
	{
		return entry_counter;
	}

	std::size_t getBufSize() const
	{
		return buf_size;
	}

	std::size_t getFreeIndex() const
	{
		for (int i = 0; i < buf_size; ++i) {
			if (buf_free[(i+rr)%buf_size]) {
				return (i+rr)%buf_size;
			}
		}
		return buf_size;
	}

	std::size_t getOldestNonFree() const
	{
		return rr;
	}

	//figure out whether index of buf is free or not
	bool isFree(unsigned int index) const
	{
		if (index >= buf_size) return false;
		if (buf_free[index]) return true;
		return false;
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
	std::size_t find(std::string name) const
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
	std::size_t push(std::string name, T entry)
	{
		return insert(name, entry);
	}

	//same as above, but replaces the oldest member even if it's not free
	//warning - if this class stores a pointer, the old pointer
	//may never be freed
	std::size_t forcePush(std::string name, T entry)
	{
		return forceInsert(name, entry);
	}

	//if "name" exists in the buffer, return it,
	//otherwise push new entry into the buffer
	std::size_t findOrPush(std::string name, T entry)
	{
		if (has(name)) {
			return find(name);
		} else {
			return push(name, entry);
		}
	}

	//only do this if u know what ur doin
	//warning - if this class stores a pointer, the old pointer
	//may never be freed
	void replace(std::size_t index, T entry)
	{
		if (isFree(index)) {
			std::cout << "ResBuf::replace error: " << index
				<< " is free, but trying to replace entry."
				<< " the dev(s) obviously didn't read the comment in ResBuf.h"
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
		freeEntry(rr);
		return buf[rr];
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
				<< printBufFree(i) << "\t"
				<< buf_names[i] << "\t" << std::endl;
		}
		return output.str();
	}
};


#endif /* SRC_RESBUF_H_ */
