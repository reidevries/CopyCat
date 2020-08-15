/*
 * resbuf_test.cpp
 *
 *  Created on: Jul 12, 2020
 *      Author: rei de vries
 */

#include <iostream>

#include "../src/ResBuf.hpp"
using namespace std;

int printPush(ResBuf<char, 7>& r, string name, char val)
{
	cout << "pushing '" << val << "' at '" << name << "'" << endl;
	return r.push(name, val);
}

int main(void)
{
	ResBuf<char, 7> test_buf('a');
	cout << test_buf.printBuf() << endl;

	cout << "pushing 't' at 'test'" << endl;
	test_buf.push("test", 't');
	cout << test_buf.printBuf() << endl;

	int a, b, c, d, e, f, g;
	
	try {
		a = printPush(test_buf, "hello", 'a');
		b = printPush(test_buf, "this", 'b');
		c = printPush(test_buf, "is", 'c');
		d = printPush(test_buf, "a test", 'd');
		e = printPush(test_buf, "for", 'e');
		f = printPush(test_buf, "resbuf", 'f');
		g = printPush(test_buf, "template", 'g');
	} catch ( const std::range_error& e ) {
		cout << "range error: " <<  e.what() << endl;
	}
	cout << test_buf.printBuf() << endl;
	
	cout << "popped char " << test_buf.pop(a) << endl;
	cout << "popped char " << test_buf.pop(c) << endl << endl;
	cout << test_buf.printBuf() << endl;
	
	printPush(test_buf, "woah overflow", '7');
	cout << test_buf.printBuf() << endl;
	
	try {
		cout << "popping char " << test_buf.pop(g) << endl;
		cout << "popping char " << test_buf.pop(f) << endl;
		cout << "popping char " << test_buf.pop(e) << endl;
		cout << "popping char " << test_buf.pop(d) << endl;
		cout << "popping char " << test_buf.pop(c) << endl;
		cout << "popping char " << test_buf.pop(b) << endl;
		cout << "popping char " << test_buf.pop(a) << endl;
	} catch ( const std::invalid_argument& e ) {
		cout << "invalid argument: " <<  e.what() << endl;
	} catch ( const std::out_of_range& e) {
		cout << "out of range error: " << e.what() << endl;
	}
	cout << test_buf.printBuf() << endl << endl;
	
	try {
		printPush(test_buf, "lets", 'z');
		printPush(test_buf, "do", 'y');
		printPush(test_buf, "it", 'x');
		printPush(test_buf, "all", 'w');
		printPush(test_buf, "again", 'v');
		printPush(test_buf, "for", 'u');
		printPush(test_buf, "the", 't');
		printPush(test_buf, "lulz", 's');
	} catch ( const std::range_error& e ) {
		cout << "range error: " <<  e.what() << endl;
	}
	cout << test_buf.printBuf() << endl;
	
	cout << "force push 'lulz'" << endl;
	int lulz = test_buf.forcePush("lulz",  's');
	cout << test_buf.printBuf() << endl << endl;
	
	cout << "replace value after 'lulz'" << endl;
	test_buf.replace(lulz, '!');
	cout << test_buf.printBuf() << endl << endl;
	
	cout << "test popOldest" << endl;
	cout << "oldest value is " << test_buf.popOldest() << endl;
	cout << test_buf.printBuf() << endl << endl;
	
	cout << "looking up 'lulz'" << endl;
	cout << "value at 'lulz' is '" << test_buf.at("lulz") << "'" << endl;
	cout << "value at previously stored index of 'lulz' is '"
		<< test_buf.at(lulz) << "'" << endl << endl;
	
	cout << "end of testing" << endl;
}



