// Calculator.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

using namespace std;

map <string, double> g_table;
int g_noOfErrors;
istream* g_input;

double GetError(const string& s)
{
	g_noOfErrors++;
	cerr << "error: " << s << '\n';
	return 1;
}

enum TokenValue
{
	NAME, NUMBER, END,
	PLUS = '+', MINUS = '-', MUL = '*', DIV = '/',
	PRINT = ';', ASSIGN = '=', LP = '(', RP = ')'
};

TokenValue g_currTok = PRINT;
string g_stringValue;
double g_numberValue;

TokenValue GetToken()
{
	char ch = 0;

	do
	{
		if(!g_input->get(ch)) return g_currTok = END;
	}while(ch != '\n' && isspace(ch));

	switch(ch)
	{
		case ';':
		case '\n':
			return g_currTok = PRINT;
		case 0:
			return g_currTok = END;
		case '*':
		case '/':
		case '+':
		case '-':
		case '(':
		case ')':
		case '=':
			return g_currTok = TokenValue(ch);
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
		case '.':
			g_input->putback(ch);
			(*g_input) >> g_numberValue;
			return g_currTok = NUMBER;
		default:
			if (isalpha (ch))
			{
				g_stringValue = ch;
				while (g_input->get(ch) &&isalnum(ch)) g_stringValue.push_back(ch);
				g_input->putback(ch);
				return g_currTok = NAME;
			}
			GetError("bad token");
			return g_currTok = PRINT;
	}
}

double GetExpression(bool get);
double GetTerm(bool get);
double GetPrimary(bool get);

double GetExpression(bool get)
{
	double left = GetTerm (get);

	for (;;)
		switch (g_currTok)
	{
		case PLUS:
			left += GetTerm(true);
			break;
		case MINUS:
			left -= GetTerm(true);
			break;
		default:
			return left;
	}
}

double GetTerm(bool get)
{
	double left = GetPrimary(get);

	for (;;)
		switch (g_currTok)
	{
		case MUL:
			left *= GetPrimary(true);
			break;
		case DIV:
			if (double d = GetPrimary(true))
			{
				left /= d;
				break;
			}
			return GetError("divide by 0");
		default:
			return left;
	}
}

double GetPrimary(bool get)
{
	if (get) GetToken();

	switch (g_currTok)
	{
	case NUMBER:
	{
				   double v = g_numberValue;
				   GetToken();
				   return v;
	}
	case NAME:
	{
				 double& v = g_table[g_stringValue];
				 if (GetToken() == ASSIGN) v = GetExpression(true);
				 return v;
	}
	case MINUS:
		return -GetPrimary(true);
	case LP:
	{
			   double e = GetExpression(true);
			   if (g_currTok != RP) return GetError("')' expected");
			   GetToken();
			   return e;
	}
	default:
		return GetError("primary expected");
	}
}

int main(int argc, char* argv[])
{
	switch (argc)
	{
	case 1:
		g_input = &cin;
		break;
	case 2:
		g_input = new istringstream (argv[1]);
		break;
	default:
		GetError("too many arguments");
		return 1;
	}

	g_table["pi"] = 3.1415926535897932385;
	g_table["e"] = 2.7182818284590452354;

	while (*g_input)
	{
		GetToken();
		if (g_currTok == END)
			break;
		if (g_currTok == PRINT)
			continue;
		cout << GetExpression(false) << '\n';
	}

	if (g_input != &cin)
		delete g_input;

	return g_noOfErrors;
}