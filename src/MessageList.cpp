/*
 * messages.cpp
 *
 *  Created on: Jun 10, 2020
 *      Author: rei de vries
 */
#include "MessageList.hpp"

using namespace std;

const string Message::type_text[] = {
	"say",
	"inspect",
	"inspect_response",
	"debug"
};

string Message::print() const
{
	stringstream ss;
	ss << "src: " << src_id
		<< " dest: " << dest_id
		<< " type: " << type_text[static_cast<int>(type)]
		<< " contents: " << message;
	return ss.str();
}

void MessageList::insert(const Message message)
{
	by_src_id[message.src_id].push_back(messages.size());
	by_dest_id[message.dest_id].push_back(messages.size());
	messages.push_back(message);
}

void MessageList::emplace(const int src_id, const int dest_id,
	const Message::Type type, const std::string message)
{
	insert(Message(src_id, dest_id, type, message));
}

void MessageList::insert(vector<Message> messages) {
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

vector<Message> MessageList::getMessagesBySrc(const int src_id) const
{
	vector<Message> accumulator;
	if (by_src_id.count(src_id) > 0) {
		for (auto const& index : by_src_id.at(src_id)) {
			accumulator.push_back(messages[index]);
		}
	}
	return accumulator;
}

vector<Message> MessageList::getMessagesByDest(const int dest_id) const
{
	vector<Message> accumulator;
	if (by_dest_id.count(dest_id) > 0) {
		for (auto& index : by_dest_id.at(dest_id)) {
			accumulator.push_back(messages[index]);
		}
	}
	return accumulator;
}
