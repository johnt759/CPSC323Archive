/* Syntax.cpp will follow the same procedure as
Reader.cpp from the first assignment, except that
each line will be processed and the symbols are
categorized by their type. */
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include "syntax.h"

using namespace std;

// Is this symbol a keyword?
bool Analyzer::isKeyword()
{
	for (int a = 0; a < 15; a++)
	{
		if (currentToken[numTokens] == keywords[a])
		{
			return true;
		}
	}
	return false;
}

// Is this symbol an operator?
bool Analyzer::isOperator()
{
	for (int d = 0; d < 8; d++)
	{
		if (currentToken[numTokens] == operators[d])
		{
			return true;
		}
	}
	return false;
}

// Is this symbol a separator?
bool Analyzer::isSeparator()
{
	for (int e = 0; e < 13; e++)
	{
		if (currentToken[numTokens] == separators[e])
		{
			return true;
		}
	}
	return false;
}

/*The switch statement procedures will follow the same
guideline as in the lexical analysis, except that the default
cases will throw an error (there is nothing to be read or an
unknown character is detected). In addition, the vertical bar
acts as a separator between each grammatical strings while the
right arrow sign (->) acts as a transition symbol.*/
void Analyzer::changeState(char s)
{
	syntaxState = currentState;	// Update and remember the last state

	switch (syntaxState)
	{
	case 0:		// Default state
		if (isalpha(s))
		{
			appendToken(s);
			currentState = 1;
		}
		else if (isdigit(s))
		{
			appendToken(s);
			currentState = 2;
		}
		else if (s == '!')
		{
			currentState = 5;
		}
		else if (s == '%')
		{
			currentState = 6;
		}
		else if (!isalnum(s))
		{
			if (isspace(s))
			{
				currentState = 0;
			}
			else
			{
				appendToken(s);
				if (isSeparator())
				{
					tokenType[numTokens] = "SEPARATOR      ";
					updateIndex();
				}
				else if (isOperator())
				{
					tokenType[numTokens] = "OPERATOR       ";
					updateIndex();
				}
			}
		}
		break;
	case 1:		// In the middle of identifier or keyword
		if (isalpha(s))
		{
			appendToken(s);
			currentState = 1;
		}
		else if (isdigit(s))
		{
			appendToken(s);
			currentState = 1;
		}
		else if (s == '$')
		{
			appendToken(s);
			currentState = 1;
		}
		else if (!isalnum(s))
		{
			if (isKeyword())
			{
				tokenType[numTokens] = "KEYWORD        ";
			}
			else
			{
				tokenType[numTokens] = "IDENTIFIER     ";
			}
			currentState = 0;
			if (isspace(s))
			{
				updateIndex();
			}
			else
			{
				updateIndex();
				changeState(s);
			}
		}
		break;
	case 2:		// Getting a numerical character
		if (isalpha(s))
		{
			appendToken(s);
			currentState = 4;
		}
		else if (isdigit(s))
		{
			appendToken(s);
			currentState = 2;
		}
		else if (!isalnum(s))
		{
			if (isspace(s))
			{
				tokenType[numTokens] = "INTEGER        ";
				currentState = 0;
				updateIndex();
			}
			else if (s == '.')
			{
				appendToken(s);
				currentState = 3;
			}
			else
			{
				tokenType[numTokens] = "INTEGER        ";
				updateIndex();
				currentState = 0;
				changeState(s);
			}
		}
		break;
	case 3:		// Found a float-point number
		if (isalpha(s))
		{
			currentState = 0;
			changeState(s);
		}
		else if (isdigit(s))
		{
			currentState = 3;
			appendToken(s);
		}
		else if (!isalnum(s))
		{
			tokenType[numTokens] = "real";
			currentState = 0;
			updateIndex();
			if (!isspace(s))
			{
				changeState(s);
			}
		}
		break;
	case 4:		// Everything else that doesn't belong in any of the categories above
		if (isalpha(s))
		{
			appendToken(s);
		}
		else if (isdigit(s))
		{
			appendToken(s);
		}
		else if (!isalnum(s))
		{
			if (isspace(s))
			{
				tokenType[numTokens] = "INVALID TOKEN  ";
				currentState = 0;
				updateIndex();
			}
			else
			{
				tokenType[numTokens] = "INVALID TOKEN  ";
				updateIndex();
				currentState = 0;
				changeState(s);
			}
		}
		break;
	case 5:		// Reading in a comment line (indicated by '!')
		if (s == '!')
		{
			currentState = 0;
		}
		break;
	case 6:
		if (s != '%')
		{
			currentState = 0;
		}
		else
		{
			currentState = 7;
		}
		break;
	}
	lastToken = s;
}

void Analyzer::updateIndex()
{
	tokenLineNumber[numTokens] = currentLineNumber;
	numTokens++;
}

void Analyzer::appendToken(char c)
{
	currentToken[numTokens] += c;
}

void Analyzer::readFromTextFile(string filename)
{
	// First, ready the input file and begin the
	// processing of each line of data in the text.
	fstream thisFile(filename);

	if (thisFile.is_open())
	{
		string currentLine;
		char thisChar;

		while (getline(thisFile, currentLine))
		{
			currentLine += ' ';
			for (int i = 0; i < currentLine.length(); i++)
			{
				thisChar = currentLine[i];
				changeState(thisChar);
				if (currentState == 7)
				{
					currentState = 0;
					break;
				}
			}
			currentLineNumber++;
		}

		thisFile.close();	// Be sure to close the file once finished.

		// Now create an output file and print all the tokens and the types
		// into the text file as well as displaying them on console.
		ofstream results;
		results.open("Output.txt");
		for (int j = 0; j < numTokens; j++)
		{
			results << tokenType[j] << " = " << currentToken[j] << "\n";
			cout << tokenType[j] << " = " << currentToken[j] << "\n";
		}
		results.close();
	}
	else
	{
		cout << "Can't open the file called " << filename << endl;
	}
}

void Analyzer::startParsing(void)
{
	// After processing through each line of text file,
	// be sure to update tokenIndex in order to read all
	// the way to the end of file.
	int tokenIndex = 0;
	while (tokenIndex < numTokens)
	{
		skipComment(tokenIndex);
		isStatement(tokenIndex);
	}
}

bool Analyzer::isStatement(int& tokenIndex)
{
	return (isAssign(tokenIndex));
}

bool Analyzer::isAssign(int& tokenIndex)
{
	if (tokenIndex > numTokens)
		return false;

	int tempNumber = tokenIndex;

	if (isIdentifier(tokenIndex)) {
		if (isEqualSymbol(tokenIndex)) {
			if (isExpression(tokenIndex)) {
				cout << "<Assign> -> <Identifier>=<Expression>;\n";
				return true;
			}
		}
	}

	tokenIndex = tempNumber;
	return false;
}

bool Analyzer::isExpression(int& tokenIndex)
{
	if (tokenIndex > numTokens)
		return false;

	int tempNumber = tokenIndex;

	if (isTerm(tokenIndex)) {
		if (isExpressionPrime(tokenIndex)) {
			cout << "<Expression> -> <Term><Expression Prime>\n";
			return true;
		}
	}

	tokenIndex = tempNumber;
	return false;
}

bool Analyzer::isExpressionPrime(int& tokenIndex)
{
	if (tokenIndex > numTokens)
		return true;

	int tempNumber = tokenIndex;

	if (isAdd(tokenIndex)) {
		if (isTerm(tokenIndex)) {
			if (isExpressionPrime(tokenIndex)) {
				cout << "<Expression Prime> -> +<Term><Expression Prime>\n";
				return true;
			}
		}
	}
	else {
		tokenIndex = tempNumber;
	}

	if (isSubtract(tokenIndex)) {
		if (isTerm(tokenIndex)) {
			if (isExpressionPrime(tokenIndex)) {
				cout << "<Expression Prime> -> -<Term><Expression Prime>\n";
				return true;
			}
		}
	}
	else {
		tokenIndex = tempNumber;
	}

	if (isEpsilon(tokenIndex)) {
		cout << "<Expression Prime> -> epsilon\n";
		return true;
	}
	else {
		tokenIndex = tempNumber;
	}

	tokenIndex = tempNumber;
	return false;
}

bool Analyzer::isTerm(int& tokenIndex)
{
	if (tokenIndex > numTokens)
		return false;

	int tempNumber = tokenIndex;

	if (isFactor(tokenIndex)) {
		if (isTermPrime(tokenIndex)) {
			cout << "<Term> -> <Factor> <Term Prime>\n";
			return true;
		}
	}

	tokenIndex = tempNumber;
	return false;
}

bool Analyzer::isTermPrime(int& tokenIndex)
{
	if (tokenIndex > numTokens)
		return true;

	int tempNumber = tokenIndex;

	if (isMultiply(tokenIndex)) {
		if (isFactor(tokenIndex)) {
			if (isTermPrime(tokenIndex)) {
				cout << "<Term Prime> -> *<Factor> <Term Prime>\n";
				return true;
			}
		}
	}
	else {
		tokenIndex = tempNumber;
	}

	if (isDivide(tokenIndex)) {
		if (isFactor(tokenIndex)) {
			if (isTermPrime(tokenIndex)) {
				cout << "<Term Prime> -> /<Factor>< Term Prime>\n";
				return true;
			}
		}
	}
	else {
		tokenIndex = tempNumber;
	}

	if (isEpsilon(tokenIndex)) {
		cout << "<Term Prime> -> epsilon\n";
		return true;
	}
	else {
		tokenIndex = tempNumber;
	}

	tokenIndex = tempNumber;
	return false;
}

bool Analyzer::isFactor(int& tokenIndex)
{
	if (tokenIndex > numTokens)
		return false;

	int tempNumber = tokenIndex;

	if (isLeftParentheses(tokenIndex)) {
		if (isExpression(tokenIndex)) {
			if (isRightParentheses(tokenIndex)) {
				cout << "<Factor> -> (<Expression>)\n";
				return true;
			}
		}
	}
	else {
		tokenIndex = tempNumber;
	}

	if (isIdentifier(tokenIndex))
	{
		cout << "<Factor> -> <Identifier>\n";
		return true;
	}
	else {
		tokenIndex = tempNumber;
	}

	tokenIndex = tempNumber;
	return false;
}

bool Analyzer::isIdentifier(int& tokenIndex)
{
	if (tokenIndex > numTokens)
		return false;

	if (tokenType[tokenIndex] == "IDENTIFIER     ")
	{
		cout << "Token: " << tokenType[tokenIndex] << "\tLexeme: " << currentToken[tokenIndex] << "\n";
		tokenIndex++;
		return true;
	}
	else {
		return false;
	}
}

bool Analyzer::isEpsilon(int& tokenIndex)
{
	if (tokenIndex > numTokens)
	{
		cout << "Token: " << tokenType[tokenIndex] << "\tLexeme: " << currentToken[tokenIndex] << "\n";
		return true;
	}
	else if (currentToken[tokenIndex] == ";")
	{
		cout << "Token: " << tokenType[tokenIndex] << "\tLexeme: " << currentToken[tokenIndex] << "\n";
		tokenIndex++;
		return true;
	}
	else {
		return true;
	}
}

bool Analyzer::isEqualSymbol(int& tokenIndex)
{
	if (tokenIndex > numTokens)
		return false;

	if (currentToken[tokenIndex] == "=")
	{
		cout << "Token: " << tokenType[tokenIndex] << "\tLexeme: " << currentToken[tokenIndex] << "\n";
		tokenIndex++;
		return true;
	}
	else {
		return false;
	}
}

bool Analyzer::isLeftParentheses(int& tokenIndex)
{
	if (tokenIndex > numTokens)
		return false;

	if (currentToken[tokenIndex] == "(")
	{
		cout << "Token: " << tokenType[tokenIndex] << "\tLexeme: " << currentToken[tokenIndex] << "\n";
		tokenIndex++;
		return true;
	}
	else {
		return false;
	}
}

bool Analyzer::isRightParentheses(int& tokenIndex)
{
	if (tokenIndex > numTokens)
		return false;

	if (currentToken[tokenIndex] == ")")
	{
		cout << "Token: " << tokenType[tokenIndex] << "\tLexeme: " << currentToken[tokenIndex] << "\n";
		tokenIndex++;
		return true;
	}
	else {
		return false;
	}
}

bool Analyzer::isAdd(int& tokenIndex)
{
	if (tokenIndex > numTokens)
		return false;

	if (currentToken[tokenIndex] == "+")
	{
		cout << "Token: " << tokenType[tokenIndex] << "\tLexeme: " << currentToken[tokenIndex] << "\n";
		tokenIndex++;
		return true;
	}
	else {
		return false;
	}
}

bool Analyzer::isSubtract(int& tokenIndex)
{
	if (tokenIndex > numTokens)
		return false;

	if (currentToken[tokenIndex] == "-")
	{
		cout << "Token: " << tokenType[tokenIndex] << "\tLexeme: " << currentToken[tokenIndex] << "\n";
		tokenIndex++;
		return true;
	}
	else {
		return false;
	}
}

bool Analyzer::isMultiply(int& tokenIndex)
{
	if (tokenIndex > numTokens)
		return false;

	if (currentToken[tokenIndex] == "*")
	{
		cout << "Token: " << tokenType[tokenIndex] << "\tLexeme: " << currentToken[tokenIndex] << "\n";
		tokenIndex++;
		return true;
	}
	else {
		return false;
	}
}

bool Analyzer::isDivide(int& tokenIndex)
{
	if (tokenIndex > numTokens)
		return false;

	if (currentToken[tokenIndex] == "/")
	{
		cout << "Token: " << tokenType[tokenIndex] << "\tLexeme: " << currentToken[tokenIndex] << "\n";
		tokenIndex++;
		return true;
	}
	else {
		return false;
	}
}

bool Analyzer::isStartOfComment(int& tokenIndex)
{
	if (tokenIndex > numTokens)
		return false;

	if ((currentToken[tokenIndex] == "!") && (currentToken[tokenIndex] == "\n"))
	{
		tokenIndex++;
		return true;
	}
	else {
		return false;
	}
}
bool Analyzer::isEndOfComment(int& tokenIndex)
{
	if (tokenIndex > numTokens)
		return false;

	if ((currentToken[tokenIndex] == "!") && (currentToken[tokenIndex] == "\n"))
	{
		tokenIndex++;
		return true;
	}
	else
	{
		return false;
	}
}
void Analyzer::skipComment(int& tokenIndex)
{
	int tempToken = tokenIndex;

	if (isStartOfComment(tokenIndex))
	{
		while (isEndOfComment(tokenIndex) == false)
		{
			cout << "Comment omitted\n";
		}
	}
	else
	{
		tokenIndex = tempToken;
	}
}