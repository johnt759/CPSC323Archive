/*Syntax.h contains the outline of the class
with the public functions and the private members.
For simplicity and clarity, only the constructors,
destructors, and the get functions are declared here;
the remainder of the code declaration goes to driver.cpp.*/
#pragma once
#include<iostream>
#include<string>
#include<cstring>

using namespace std;

class Analyzer
{
public:
	// Treat ampersand (&) as an epislon and put it along with terminal symbols.
	Analyzer()
	{
		numTokens = 0;
		lastToken = '\0';
		currentLineNumber = 1;	// Start at the top of the text file.
		currentToken = new string[1000];
		tokenType = new string[1000];
		tokenLineNumber = new int[1000];
		keywords = new string[15]{ "int", "float", "bool", "if", "else",
								  "then", "do", "while", "whileend", "do",
								  "doend", "for", "and", "or", "function" };
		operators = new string[8]{ "*", "+", "-", "=", "/", ">", "<", "%" };
		separators = new string[13]{ "'", "(", ")", "{", "}", "[", "]", ",", ".", ":", ";", "$", "!" };
		currentState = 0;
		syntaxState = 0;
		nKeywords = 15;
		nSeparators = 13;
		nOperators = 8;
	}
	~Analyzer()
	{
		delete[] currentToken;
		delete[] tokenType;
		delete[] operators;
		delete[] separators;
	}
	int getIndex()
	{
		return numTokens;
	}
	string* getToken()
	{
		return currentToken;
	}
	string* getTokenType()
	{
		return tokenType;
	}
	int* getTokenLineNumber()
	{
		return tokenLineNumber;
	}
	bool isKeyword();
	bool isOperator();
	bool isSeparator();
	void changeState(char s);
	void appendToken(char c);
	void readFromTextFile(string filename);
	void updateIndex();

	// The following functions below will do the
	// parsing by the grammatical rules.
	void startParsing(void);
	bool isStatement(int& tokenIndex);
	bool isAssign(int& tokenIndex);
	bool isExpression(int& tokenIndex);
	bool isExpressionPrime(int& tokenIndex);
	bool isTerm(int& tokenIndex);
	bool isTermPrime(int& tokenIndex);
	bool isFactor(int& tokenIndex);
	bool isIdentifier(int& tokenIndex);
	bool isEpsilon(int& tokenIndex);
	bool isEqualSymbol(int& tokenIndex);
	bool isLeftParentheses(int& tokenIndex);
	bool isRightParentheses(int& tokenIndex);
	bool isAdd(int& tokenIndex);
	bool isSubtract(int& tokenIndex);
	bool isMultiply(int& tokenIndex);
	bool isDivide(int& tokenIndex);
	// For these following functions below, we will ignore everything in the comment section.
	bool isStartOfComment(int& tokenIndex);
	bool isEndOfComment(int& tokenIndex);
	void skipComment(int& tokenIndex);
private:
	int nKeywords;
	int nSeparators;
	int nOperators;
	int currentLineNumber;
	string* currentToken;
	string* tokenType;
	string* operators;
	string* separators;
	string* keywords;
	int* tokenLineNumber;
	int numTokens, currentState, syntaxState;
	char lastToken;
};
