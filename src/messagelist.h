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

enum MessageType{say, inspect, inspect_response};

struct Message
{
	int src_id;
	int dest_id;
	MessageType type;
	std::string message;
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
	void insert(const std::vector<Message> messages);

	std::vector<Message> popAllMessages();
	std::vector<Message> getMessagesBySrc(int src_id);
	std::vector<Message> getMessagesByDest(int dest_id);

};


#endif /* MESSAGELIST_H_ */
