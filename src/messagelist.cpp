/*
 * messages.cpp
 *
 *  Created on: Jun 10, 2020
 *      Author: rei de vries
 */
#include "messagelist.h"

using namespace std;

void MessageList::insert(const Message message)
{
	by_src_id[message.src_id].push_back(messages.size());
	by_dest_id[message.dest_id].push_back(messages.size());
	messages.push_back(message);
}

void MessageList::insert(const vector<Message> messages) {
	for (auto const& message : messages) insert(message);
}

vector<Message> MessageList::popAllMessages()
{
	vector<Message> popped = messages;
	messages.clear();
	by_dest_id.clear();
	by_src_id.clear();
	return popped;
}

vector<Message> MessageList::getMessagesBySrc(int src_id)
{
	vector<Message> accumulator;
	if (by_src_id.count(src_id) > 0) {
		for (auto const& index : by_src_id.at(src_id)) {
			accumulator.push_back(messages[index]);
		}
	}
	return accumulator;
}

vector<Message> MessageList::getMessagesByDest(int dest_id)
{
	vector<Message> accumulator;
	if (by_src_id.count(dest_id) > 0) {
		for (auto const& index : by_src_id.at(dest_id)) {
			accumulator.push_back(messages[index]);
		}
	}
	return accumulator;
}
