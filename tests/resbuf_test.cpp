/*
 * resbuf_test.cpp
 *
 *  Created on: Jul 12, 2020
 *      Author: rei de vries
 */

#include <iostream>
#include "../src/resbuf.h"
using namespace std;

int main(void)
{
	ResBuf<char, 8> test_buf('a');
	cout << test_buf.printBuf() << endl;

	test_buf.push("test", 't');
	cout << test_buf.printBuf() << endl;

	int hello = test_buf.push("hello", '0');
	int this_int = test_buf.push("this", '1');
	int is = test_buf.push("is", '2');
	int a_test = test_buf.push("a test", '3');
	int for_int = test_buf.push("for", '4');
	int res_buf = test_buf.push("resbuf", '5');
	int template_int = test_buf.push("template", '6');
	cout << test_buf.printBuf() << endl;
	cout << "popped char " << test_buf.pop(template_int) << endl << endl;
	cout << "popped char " << test_buf.pop(this_int) << endl << endl;
	test_buf.push("woah overflow", '7');
	cout << test_buf.printBuf() << endl;
	cout << "popped char " << test_buf.pop(hello) << endl << endl;
	cout << "popped char " << test_buf.pop(this_int) << endl << endl;
	cout << "popped char " << test_buf.pop(is) << endl << endl;
	cout << "popped char " << test_buf.pop(a_test) << endl << endl;
	cout << "popped char " << test_buf.pop(for_int) << endl << endl;
	cout << "popped char " << test_buf.pop(res_buf) << endl << endl;
	cout << "popped char " << test_buf.pop(template_int) << endl << endl;
	test_buf.push("lets", 'z');
	test_buf.push("do", 'y');
	test_buf.push("it", 'x');
	test_buf.push("all", 'w');
	test_buf.push("again", 'v');
	test_buf.push("for", 'u');
	test_buf.push("the", 't');
	test_buf.push("lulz", 's');
	cout << test_buf.printBuf() << endl;
}



