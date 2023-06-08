/*Driver.cpp is where the main testing of the
syntax analysis takes place.*/
#include<iostream>
#include<fstream>
#include<string>
#include "syntax.h"
using namespace std;

int main()
{
	string tester1 = "SampleInput1.txt";
	string tester2 = "SampleInput2.txt";
	string tester3 = "test1assignmentOperator.txt";
	Analyzer thisInput;
	
	//thisInput.readFromTextFile(tester1);
	//thisInput.readFromTextFile(tester2);
	thisInput.readFromTextFile(tester3);
	thisInput.startParsing();

	return 0;
}
