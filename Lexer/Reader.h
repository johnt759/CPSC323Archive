// Reader.h contains the outline of the class
// with the public functions and private members.
// Only the constructor and destructor are declared
// in this header file; the rest of the declaration
// for all member functions goes to Reader.cpp.
#pragma once
#include<iostream>
#include<string>
#include<cstring>
//#include "Analyzer.h"
using namespace std;

class Reader
{
public:
	// Constructor initializes and allocates the memory for all dynamic variables.
	Reader()
	{
		lineNumber = 1;	// Assume we are at the beginning of the text file.
		token = new string[100000];
		tokenType = new string[100000];
		tokenLineNumber = new int[100000];

		keywords = new string[15]{"int", "float", "bool", "if", "else",
								  "then", "do", "while", "whileend", "do",
								  "doend", "for", "and", "or", "function"};
		nKeywords = 15;

		separators = new string[13]{"'", "(", ")", "{", "}", "[", "]", ",", ".", ":", ";", "$", "!"};
		nSeparators = 13;

		operators = new string[8]{"*", "+", "-", "=", "/", ">", "<", "%"};
		nOperators = 8;

		currentState = 0;
		lexiconState = 0;
	}

	// Destructor cleans up the allocated memory for the dynamic variables.
	~Reader()
	{
		delete[] token;
		delete[] tokenType;
		delete[] operators;
		delete[] separators;
	}

	void LexiconResultFileFromData(string filename);

	int getNumberOfTokens()
	{
		return whichIndex;
	}

	string *getToken()
	{
		return token;
	}

	string *getTokenType()
	{
		return tokenType;
	}

	int *getTokenLineNumber()
	{
		return tokenLineNumber;
	}

	void addToLexicon(char c);
	// The 3 boolean functions below will determine whether the token
	// is a keyword or a separator.
	bool isKeyword();
	bool isSeparator();
	bool isOperator();
	// The 2 functions below will adjust the current state and index
	// according to the results of the boolean functions above.
	void updateState(char status);
	void updateIndex();

private:
	int nKeywords;
	int nSeparators;
	int nOperators;

	int lineNumber;
	string *token;
	string *tokenType;
	string *keywords;
	string *operators;
	string *separators;
	int *tokenLineNumber;
	int whichIndex, currentState, lexiconState;
	char lastToken;
};