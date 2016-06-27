#include "stdafx.h"
#include "CalcLib.h"

using namespace std;

double getTerm(Application &app, bool get);
double getPrimary(Application &app, bool get);

double getError(Application &app, const string& s)
{
	app.noOfErrors++;
	cerr << "error: " << s << '\n';
	return 1;
}

void getToken(Application &app)
{
	char ch = 0;

	do
	{
		if (!app.input->get(ch))
		{
			app.currTok = END;
			return;
		}
	} while (ch != '\n' && isspace(ch));

	switch (ch)
	{
	case ';':
	case '\n':
		app.currTok = PRINT;
		return;
	case 0:
		app.currTok = END;
		return;
	case '*':
	case '/':
	case '+':
	case '-':
	case '(':
	case ')':
	case '=':
		app.currTok = TokenValue(ch);
		return;
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	case '.':
		app.input->putback(ch);
		(*app.input) >> app.numberValue;
		app.currTok = NUMBER;
		return;
	default:
		if (isalpha(ch))
		{
			app.stringValue = ch;
			while (app.input->get(ch) && isalnum(ch))
			{
				app.stringValue.push_back(ch);
			}
			app.input->putback(ch);
			app.currTok = NAME;
			return;
		}
		getError(app, "bad token");
		app.currTok = PRINT;
		return;
	}
}

double getExpression(Application &app, bool get)
{
	double left = getTerm(app, get);

	for (;;)
		switch (app.currTok)
	{
		case PLUS:
			left += getTerm(app, true);
			break;
		case MINUS:
			left -= getTerm(app, true);
			break;
		default:
			return left;
	}
}

double getTerm(Application &app, bool get)
{
	double left = getPrimary(app, get);

	for (;;)
		switch (app.currTok)
	{
		case MUL:
			left *= getPrimary(app, true);
			break;
		case DIV:
			if (double d = getPrimary(app, true))
			{
				left /= d;
				break;
			}
			return getError(app, "divide by 0");
		default:
			return left;
	}
}

double getPrimary(Application &app, bool get)
{
	if (get)
	{
		getToken(app);
	}

	switch (app.currTok)
	{
	case NUMBER:
	{
				   double v = app.numberValue;
				   getToken(app);
				   return v;
	}
	case NAME:
	{
				 double& v = app.table[app.stringValue];
				 getToken(app);
				 if (app.currTok == ASSIGN)
				 {
					 v = getExpression(app, true);
				 }
				 return v;
	}
	case MINUS:
		return -getPrimary(app, true);
	case LP:
	{
			   double e = getExpression(app, true);
			   if (app.currTok != RP)
			   {
				   return getError(app, "')' expected");
			   }
			   getToken(app);
			   return e;
	}
	default:
		return getError(app, "primary expected");
	}
}