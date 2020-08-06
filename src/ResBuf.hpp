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
#include <stdexcept>

template <class T, std::size_t buf_size>
class ResBuf {
public:
	static constexpr const char* free_entry_name = "_empty";
	
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
	
	void setRR(int rr)
	{
		this->rr = (rr+1) % buf_size;
	}

	//constant time
	void release(std::size_t i)
	{
		entry_counter -= 1-static_cast<int>(buf_free[i]);
		buf_free[i] = true;
		buf_by_name.erase(buf_names[i]);
		buf_names[i] = free_entry_name;
	}

	//constant time, inserts into a specific index
	void insert(std::size_t i, std::string name, T entry)
	{
		//no increment will occur if buf is not free, without branching
		entry_counter += static_cast<int>(buf_free[i]);
		buf_free[i] = false;
		buf_by_name.insert({name, i});
		buf_names[i] = name;
		buf[i] = entry;
	}

	//linear time, down to constant time if entries are never released
	std::size_t insert(std::string name, T entry)
	{
		std::size_t index = getFreeIndex();
		insert(index, name, entry);
		setRR(index);
		return index;
	}

	//constant time, inserts based on round robin counter without finding
	//a free index
	std::size_t forceInsert(std::string name, T entry)
	{
		std::size_t index = rr;
		insert(index, name, entry);
		incRR();
		return index;
	}

	//for printing debug output
	const char* printBufFree(std::size_t i)
	{
		return (buf_free[i])?"true":"false";
	}

public:
	ResBuf()
	{
		buf_free.fill(buf_size);
		buf_names.fill(free_entry_name);
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
	
	std::array<std::string, buf_size> getNames() const
	{
		return buf_names;
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
	bool isFree(std::size_t index) const
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
	
	//logn time access
	T get(const std::string& name) const
	{
		return buf[buf_by_name.at(name)];
	}

	//constant time access
	T& at(std::size_t index)
	{
		return buf.at(index);
	}

	//logn time access
	T& at(const std::string& name)
	{
		return buf[buf_by_name.at(name)];
	}

	//logn time
	std::size_t find(const std::string& name) const
	{
		return buf_by_name.at(name);
	}

	//figure out whether buf has a certain name in logn time
	bool has(const std::string& name) const
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
		if (entry_counter >= buf_size) {
			throw std::range_error("no free space in buf");
		}
		if (buf_by_name.count(name) > 0) {
			throw std::invalid_argument("buf already contains " + name
				+ ", use pushOrReplace(string, T) to replace it");
		}
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
			throw std::invalid_argument("index of " + std::to_string(index) 
				+ " is free, obviously the devs didn't read the comment");
			return;
		}
		buf.at(index) = entry;
	}
	
	std::size_t pushOrReplace(std::string name,  T entry)
	{
		if (has(name)) {
			int index = find(name);
			replace(index, entry);
			return index;
		} else {
			return push(name, entry);
		}
	}

	//fetches a reference to the element at the given index
	//and sets it to be free.
	//MUST unload referenced object externally to avoid leaks
	//should be constant time
	T& pop(std::size_t index)
	{
		if (index > buf_size) {
			throw std::out_of_range("index " + std::to_string(index)
				+ "exceeds buf size of " + std::to_string(buf_size));
		}

		if (isFree(index)) {
			throw std::invalid_argument("index of " + std::to_string(index) 
				+ " is already free");
		}

		release(index);
		return buf[index];
	}

	//run pop(getOldestNonFree())
	T& popOldest() {
		release(rr);
		return buf[rr];
	}

	std::array<T, buf_size>& popAll()
	{
		for (int i = 0; i < buf_size; ++i) {
			release(i);
		}
		return buf;
	}

	void clear()
	{
		for (int i = 0; i < buf_size; ++i) {
			release(i);
		}
	}

	//for debugging
	std::string printBuf() {
		std::stringstream output;
		output << "index:\tvalue:\tfree:\tname:\t" << std::endl;
		for (int i = 0; i < buf_size; ++i) {
			output << i << "\t"
				<< buf[i] << "\t"
				<< printBufFree(i) << "\t"
				<< buf_names[i] << "\t";
			if (i == rr) output <<  "*";
			output << std::endl;
		}
		return output.str();
	}
};


#endif /* SRC_RESBUF_H_ */
