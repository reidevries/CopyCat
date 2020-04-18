#include "debugprinter.h"

void DebugPrinter::printDebug(std::string source, std::string debugmsg, int verbosity) {
  if (verbosity <= VERBOSITY) {
    std::cout << source << ": " << debugmsg << std::endl;
  }
}

void DebugPrinter::printDebug(const char* source, const char* debugmsg, int verbosity) {
	printDebug(std::string(source), std::string(debugmsg), verbosity);
}

void DebugPrinter::printDebug(const char* source, uintptr_t ref, const char* debugmsg, int verbosity) {
	std::stringstream ss;
	ss << ref << debugmsg;
	printDebug(std::string(source), ss.str(), verbosity);
}

void DebugPrinter::saveDebug(std::string source, std::string debugmsg, std::string filename) {
  std::ofstream f;
  f.open(filename);
  if (f) {
    f << source << ": " << debugmsg << std::endl;
    f.close();
  } else {
    std::stringstream ss;
    ss << "Error opening file " << filename << " to save debug log from " << source << std::endl;
    printDebug("DebugPrinter::saveDebug", ss.str().c_str(), 1);
    return;
  }
}
