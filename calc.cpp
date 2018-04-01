#include "std_lib_facilities.h"

using namespace std;

/* Grammer
EXPRESSION ->
	TERM
	EXPRESSION + TERM
	EXPRESSION - TERM
TERM ->
	PRIMARY
	TERM * PRIMARY
	TERM / PRIMARY
	TERM % PRIMARY
PRIMARY ->
	NUMBER
	( EXPRESSION )
NUMBER ->
	floating point literal
*/


class Token {
public:
	char kind;
	double value;
};

Token get_token();
double expression();
double term();
double primary();

double expression() {
	double left = term();
	Token t = ts.get();
	while(true) {
		switch(t.kind) {
			case '+':
				left += term();
				t = ts.get();
				break;
			case '-':
				left -= term();
				t = ts.get();
				break;
			default:
				ts.putback(t);
				return left;
		}
	}
}

double term() {
	double left = primary();
	Token t = ts.get();
	while(true) {
		switch(t.kind) {
			case '*':
				left *= primary();
				t = ts.get();
				break;
			case '/':
			{
				double d = primary();
				if (d == 0) error("divide by zero!");
				left /= d; 
				t = ts.get();
				break;
			}
			default:
				ts.putback(t);
				return left;
		}
	}
}

double primary() {
	Token t = get_token();
	switch (t.kind) {
		case '(':
		{
			double d = expression();
			t = get_token();
			if (t.kind != ')') error("')' expected");
			return 0;
		}
		case '8':
			return t.value;
		default:
			error("primary expected");
	}	
}

int main() {
	try {
		double val = 0;
		while(cin) {
			Token t = ts.get();

			if (t.kind == 'q') break;
			if (t.kind == ';')
				cout << "=" << val << "\n";
			else
				ts.putback(t);
			val = expression();
		}
	}
	catch (exception& e) {
		cerr << e.what() << "\n";
		return 1;
	}
	catch (...) {
		cerr << "exception\n";
		return 2; 
	}
}
