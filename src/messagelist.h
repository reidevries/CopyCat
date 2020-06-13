/*
 * messagelist.h
 *
 *  Created on: Jun 10, 2020
 *      Author: rei de vries
 *	A message struct for message-passing
 */

#ifndef MESSAGELIST_H_
#define MESSAGELIST_H_

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <sstream>
#include "debugprinter.h"

struct Message
{
	enum Type{say, inspect, inspect_response, debug};
	static const std::string type_text[];

	const int src_id; 	//0 for system messages
	const int dest_id;	//also 0 for system messages
	const Type type;
	const std::string message;
	Message(int set_src_id,
		int set_dest_id,
		Message::Type set_type,
		std::string set_message)
		: src_id(set_src_id),
		  dest_id(set_dest_id),
		  type(set_type),
		  message(set_message)
	{
	}

	std::string print() const;
};

class MessageList
{
private:
	std::vector<Message> messages;
	std::map<int, std::vector<int>> by_src_id;
	std::map<int, std::vector<int>> by_dest_id;

public:
	MessageList() {};
	void insert(const Message message);
	void emplace(const int src_id, const int dest_id,
		const Message::Type type, const std::string message);
	void insert(const std::vector<Message> messages);

	std::vector<Message> popAllMessages();
	std::vector<Message> getMessagesBySrc(const int src_id);
	std::vector<Message> getMessagesByDest(const int dest_id);


};


#endif /* MESSAGELIST_H_ */
