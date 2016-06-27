#ifndef CALCLIB_H
#define CALCLIB_H

enum TokenValue
{
	NAME, NUMBER, END,
	PLUS = '+', MINUS = '-', MUL = '*', DIV = '/',
	PRINT = ';', ASSIGN = '=', LP = '(', RP = ')'
};

struct Application
{
	std::map <std::string, double> table;
	int noOfErrors;
	std::istream* input;
	TokenValue currTok;
	std::string stringValue;
	double numberValue;
};

double getError(Application &app, const std::string& s);
void getToken(Application &app);
double getExpression(Application &app, bool get);

#endif