#include "DebugPrinter.hpp"

using namespace std;

stringstream DebugPrinter::record("start log\n");
string DebugPrinter::last_log("");
int DebugPrinter::repetition_count = 0;

void DebugPrinter::printDebug(int verbosity,
	string src, string debug_msg)
{
	if (verbosity <= ReiDV::VERBOSITY) {
		if (verbosity == 4) cout << "DEBUG: ";
		string log_line = src + ": " + debug_msg;
		if (log_line == last_log) {
			cout << "\r" << " [" 
				<< ++repetition_count << "]"
				<< flush;
			DebugPrinter::record << "\r" << " [" 
				<< repetition_count << "]"
				<< flush;
		} else {
			last_log = log_line;
			repetition_count = 0;
			cout << log_line << endl;
			DebugPrinter::record << log_line << endl;
		}
	}
}

void DebugPrinter::printDebug(int verbosity,
	const char* src, const char* debug_msg)
{
	printDebug(verbosity, string(src), string(debug_msg));
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
