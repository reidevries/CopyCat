#ifndef DEBUGPRINTER_H
#define DEBUGPRINTER_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

//verbosity controls how many log messages there are
//0 is for critical errors, 1 is for general errors, 2 is for general messages, 3 is for extra messages, 4 is for tests/outputs that shouldn't usually be seen

class DebugPrinter {
  public:
	static const int VERBOSITY = 2;
    static void printDebug(std::string source, std::string debugmsg, int verbosity);
    static void printDebug(const char* source, const char* debugmsg, int verbosity);
    static void printDebug(const char* source, uintptr_t ref, const char* debugmsg, int verbosity);
    static void saveDebug(std::string source, std::string debugmsg, std::string filename);
};

#endif
