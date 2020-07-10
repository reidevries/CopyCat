#include "debugprinter.h"

using namespace std;

stringstream DebugPrinter::record("start log\n");

void DebugPrinter::printDebug(int verbosity,
	string source,
	string debugmsg)
{
	if (verbosity <= VERBOSITY) {
		if (verbosity == 4) cout << "DEBUG: ";
		cout << source << ": " << debugmsg << endl;
		DebugPrinter::record << source << ": " << debugmsg << endl;
	}
}

void DebugPrinter::printDebug(int verbosity,
	const char* source,
	const char* debugmsg)
{
	printDebug(verbosity, string(source), string(debugmsg));
}

void DebugPrinter::saveDebugLog(const char* filename)
{
  ofstream f;
  f.open(filename);
  if (f) {
    f << DebugPrinter::record.str();
    f.close();
  } else {
    printDebug(1, "DebugPrinter::saveDebugLog",
    	string("Error opening file ") + filename + " to save debug log");
    return;
  }
}
