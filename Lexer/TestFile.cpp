// TestFile.cpp is where the main testing of the
// lexical analysis takes place.
// Credit to Matthew Camarena's sample code from Github
// https://github.com/MatthewCamarena/Rat18F
#include<iostream>
#include<fstream> // Needed to open the text file.
#include<string> // Needed to work with strings.
#include "Reader.h"
//#include "Analyzer.h"
using namespace std;

int main()
{
	// First ready the test file called testLex.txt.
	// Then use the Reader class variable to begin the
	// lexicon deconstruction and write down the results
	// into another text file. After that, have the Analyzer
	// scanner breakdown the text file into lexemes seperated
	// by token types.
	string testFile = "testLex.txt";
	string testFile1 = "SampleInputFile1.txt";
	Reader results;
	results.LexiconResultFileFromData(testFile1);
	//results.LexiconResultFileFromData(testFile);
	//Analyzer scanner;
	//scanner.execute(testFile);
	//scanner.execute(testFile1);
	system("pause");
	return 0;
}