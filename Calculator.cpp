// Calculator.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "CalcLib.h"

using namespace std;

int main(int argc, char* argv[])
{
	Application app;
	app.currTok = PRINT;
	app.noOfErrors = 0;

	switch (argc)
	{
	case 1:
		app.input = &cin;
		break;
	case 2:
		app.input = new istringstream (argv[1]);
		break;
	default:
		getError(app, "too many arguments");
		return 1;
	}

	app.table["pi"] = 3.1415926535897932385;
	app.table["e"] = 2.7182818284590452354;

	while (*app.input)
	{
		getToken(app);
		if (app.currTok == END)
			break;
		if (app.currTok == PRINT)
			continue;
		cout << getExpression(app, false) << '\n';
	}

	if (app.input != &cin)
		delete app.input;

	return app.noOfErrors;
}