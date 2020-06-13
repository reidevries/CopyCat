#include "debugprinter.h"

using namespace std;

void DebugPrinter::printDebug(string source, string debugmsg, int verbosity) {
	if (verbosity <= VERBOSITY) {
		cout << source << ": " << debugmsg << endl;
		//log << source << ": " << debugmsg << endl;
	}
}

void DebugPrinter::printDebug(const char* source, const char* debugmsg, int verbosity) {
	printDebug(string(source), string(debugmsg), verbosity);
}

void DebugPrinter::printDebug(const char* source, uintptr_t ref, const char* debugmsg, int verbosity) {
	stringstream ss;
	ss << ref << debugmsg;
	printDebug(static_cast<string>(source), ss.str(), verbosity);
}

void DebugPrinter::saveDebugLog(const char* filename) {
  ofstream f;
  f.open(filename);
  if (f) {
    //f << log.str();
    f.close();
  } else {
    stringstream ss;
    ss << "Error opening file " << filename << " to save debug log" << endl;
    printDebug("DebugPrinter::saveDebug", ss.str().c_str(), 1);
    return;
  }
}
