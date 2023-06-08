// Reader.cpp will read through the text file and
// determine the category of each token when doing
// the lexicon analysis.
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include "Reader.h"
using namespace std;

bool Reader::isKeyword()
{
	for (int x = 0; x < nKeywords; x++)
	{
		if (token[whichIndex] == keywords[x])
		{
			return true;
		}
	}
	return false;
}

bool Reader::isSeparator()
{
	for (int y = 0; y < nSeparators; y++)
	{
		if (token[whichIndex] == separators[y])
		{
			return true;
		}
	}
	return false;
}

bool Reader::isOperator()
{
	for (int y = 0; y < nOperators; y++)
	{
		if (token[whichIndex] == operators[y])
		{
			return true;
		}
	}
	return false;
}

/*
In this function, perform the following switch statements based on the
lexicon's current state number. Assuming there are no default cases,
determine whether the reader character belongs to an alphabet,
a numeric, or other character type.
*/
void Reader::updateState(char st)
{
	lexiconState = currentState;	// Update the last state

	switch (lexiconState)
	{
	case 0:		// Default state
		if (isalpha(st))
		{
			addToLexicon(st);
			currentState = 1;
		}
		else if (isdigit(st))
		{
			addToLexicon(st);
			currentState = 2;
		}
		else if (st == '!')
		{
			currentState = 5;
		}
		else if (!isalnum(st))
		{
			if (isspace(st))
			{
				currentState = 0;
			}
			else
			{
				addToLexicon(st);
				if (isSeparator())
				{
					tokenType[whichIndex] = "SEPARATOR      ";
					updateIndex();
				}
				else if (isOperator())
				{
					tokenType[whichIndex] = "OPERATOR       ";
					updateIndex();
				}
			}
		}
		break;
	case 1:		// In the middle of identifier or keyword
		if (isalpha(st))
		{
			addToLexicon(st);
			currentState = 1;
		}
		else if (isdigit(st))
		{
			addToLexicon(st);
			currentState = 1;
		}
		else if (st=='$')
		{
			addToLexicon(st);
			currentState = 1;
		}
		else if (!isalnum(st))
		{
			if (isKeyword())
			{
				tokenType[whichIndex] = "KEYWORD        ";
			}
			else
			{
				tokenType[whichIndex] = "IDENTIFIER     ";
			}
			currentState = 0;
			if (isspace(st))
			{
				updateIndex();
			}
			else
			{
				updateIndex();
				updateState(st);
			}
		}
		break;
	case 2:		// Getting a numerical character
		if (isalpha(st))
		{
			addToLexicon(st);
			currentState = 4;
		}
		else if (isdigit(st))
		{
			addToLexicon(st);
			currentState = 2;
		}
		else if (!isalnum(st))
		{
			if (isspace(st))
			{
				tokenType[whichIndex] = "INTEGER        ";
				currentState = 0;
				updateIndex();
			}
			else if (st == '.')
			{
				addToLexicon(st);
				currentState = 3;
			}
			else
			{
				tokenType[whichIndex] = "INTEGER        ";
				updateIndex();
				currentState = 0;
				updateState(st);
			}
		}
		break;
	case 3:		// Found a float-point number
		if (isalpha(st))
		{
			currentState = 0;
			updateState(st);
		}
		else if (isdigit(st))
		{
			currentState = 3;
			addToLexicon(st);
		}
		else if (!isalnum(st))
		{
			tokenType[whichIndex] = "real";
			currentState = 0;
			updateIndex();
			if (!isspace(st))
			{
				updateState(st);
			}
		}
		break;
	case 4:		// Everything else that doesn't belong in any of the categories above
		if (isalpha(st))
		{
			addToLexicon(st);
		}
		else if (isdigit(st))
		{
			addToLexicon(st);
		}
		else if (!isalnum(st))
		{
			if (isspace(st))
			{
				tokenType[whichIndex] = "INVALID TOKEN  ";
				currentState = 0;
				updateIndex();
			}
			else
			{
				tokenType[whichIndex] = "INVALID TOKEN  ";
				updateIndex();
				currentState = 0;
				updateState(st);
			}
		}
		break;
	case 5:		// Reading in a comment line (indicated by '!')
		if (st == '!')
		{
			currentState = 0;
		}
		break;
	}
	lastToken = st;
}

void Reader::addToLexicon(char c)
{
	token[whichIndex] += c;
}

void Reader::updateIndex()
{
	tokenLineNumber[whichIndex] = lineNumber;
	whichIndex++;
}

void Reader::LexiconResultFileFromData(string filename)
{
	// First, ready the input file and if possible,
	// process through each line of data and store
	// each string for later use.
	fstream readerFile(filename);

	if (readerFile.is_open())
	{
		string currentLine;
		char readerChar;

		while (getline(readerFile, currentLine))
		{
			currentLine += ' ';
			for (int e = 0; e < currentLine.length(); e++)
			{
				readerChar = currentLine[e];
				updateState(readerChar);
			}
			lineNumber++;
		}
		readerFile.close();

		// Now create a new output file titled "Results" and 
		// write down the stored lines of string into the text file.
		ofstream writerFile;
		//writerFile.open("Output.txt");
		writerFile.open("Output1.txt");
		for (int j = 0; j < whichIndex; j++)
		{
			writerFile << tokenType[j] << " = " << token[j] << "\n";
			cout << tokenType[j] << " = " << token[j] << "\n";
		}
		writerFile.close();
	}
	else
	{
		cout << "Can't open the file called " + filename + ".\n";
	}
}