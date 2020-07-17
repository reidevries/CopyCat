#ifndef DEBUGPRINTER_H
#define DEBUGPRINTER_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

//verbosity controls how many log messages there are
//0 is for critical errors, 1 is for general errors,
//2 is for general messages, 3 is for extra messages,
//4 is for tests/outputs that shouldn't usually be seen
#include "catconf.h"
#ifndef CATCONF_H
#define CAT_VERBOSITY 0
#endif

class DebugPrinter {
public:
	static const int VERBOSITY = CAT_VERBOSITY;
	static std::stringstream record;
	//use the below two variables to concatenate rapidly repeating log msgs
	static std::string last_log;
	static int repetition_count;
	//prints a debugmsg like "source: debugmsg" if verbosity <= VERBOSITY
    static void printDebug(int verbosity,
		std::string source, std::string debugmsg);
	static void printDebug(int verbosity,
		const char* source, const char* debugmsg);
	static void saveDebugLog(const char* filename);
};

#endif
