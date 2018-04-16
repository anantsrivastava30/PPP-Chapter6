#include "calc.h"

using namespace std;

/* Grammer

CALCULATION ->
	STATEMENT 
	PRINT 
	QUIT
	CALCULATION STATEMENT 

STATEMENTS ->
	DECLARATION 
	EXPRESSION

DECLARATION ->
	"LET" NAME "=" EXPRESSION

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
	NAME
	( EXPRESSION )
	{ EXPRESSION }
	- PRIMARY
	+ PRIMARY
NUMBER ->
	floating point literal
NAME ->
	var
*/


Token_stream::Token_stream()
:full(false), buffer(0)    // no Token in buffer
{
}

void Token_stream::putback(Token t){
	if (full) error("putback into a full buffer");
	buffer = t;
	full = true;
}

Token Token_stream::get() {
	if (full) {
		full = false;
		return buffer;
	}
	char ch;
	cin >> ch;

	switch(ch){
		case ';':
 		case 'q':
 		case '(': 
 		case ')': 
 		case '+': 
 		case '-': 
 		case '*': 
 		case '/': 
 		case '{': 
 		case '}': 
 		case '%':
 		case '=':
 			return Token(ch);
 		case '.':
 		case '0': case '1': case '2': case '3': case '4':
 		case '5': case '6': case '7': case '8': case '9': 
 		{
 			cin.putback(ch);
 			double val;
 			cin >> val;
 			return  Token(number, val);
 		}
 		default:
 			if (isalpha(ch)) {
 				string s;
 				s += ch;
 				while (cin.get(ch) && (isalpha(ch) || isdigit(ch))) s += ch;
 				cin.putback(ch);
 				if(s == declkey) return Token(let);
 				return Token(name, s);
 			}
 			error("bad token");
	}
}

// working as expected
double get_value(string s) {
	// cout << "size of vector : " << var_table.size() << endl;
	for (const Variable& v : var_table) {
		// cout << " in get_value before if " << v.name << s << v.value << endl;
		if (v.name == s) return v.value;
	}
	error("get : undefined variable ", s);
}

void set_value(string s, double d) {
	for (Variable& v : var_table) {
		if (v.name == s) {
			v.value = d;
			return;
		}
	}
	error("set the variable before assignment", s);
}

bool is_declared(string var) {
	for (const Variable&v : var_table)
		if (v.name == var) return true;
	return false;
}

// Symbol table
double define_name(string var, double val) {
	if (is_declared(var)) error(var, " declared twice");
	var_table.push_back(Variable(var, val));
	return val;
}

double statement() {
	Token t = ts.get();
	switch(t.kind) {
	case let:
		return declaration();
	default:
		ts.putback(t);
		return expression();
	}
}

double declaration(){
	Token t = ts.get();
	if(t.kind != name) error("name expected in declaration");
	string var_name = t.name;

	Token t2 = ts.get();
	if (t2.kind != '=') error("= missing in declaration of", var_name);

	double d = expression();
	define_name(var_name, d);
	// cout << get_value(var_name) << " : value of :" << var_name << endl;
	return d;
}

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
			case '%':
			{
				double d = primary();
				if(d == 0) error("%:divide by zero");
				left = fmod(left,d);
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
	Token t = ts.get();
	// cout << t.kind << "\n";
	switch (t.kind) {
		case '(':
		{
			double d = expression();
			t = ts.get();
			if (t.kind != ')') error("')' expected");
			return d;
		}
		case '{':
		{
			double d = expression();
			t = ts.get();
			if (t.kind != '}') error("} expected");
			return d;
		}
		case number:
			return t.value;
		case '-':
			return - primary();
		case '+':
			return primary();
		case name:
		{
			// cout << t.name << "\n";

			double d = get_value(t.name);

			return d;
			// return get_value(var);
		}
		default:
			error("primary expected");
	}	
}

// TODO add feature for printing only the expression not the 
// declaration  
void calculate()
{
double var;
while(cin) {

	cout << "> ";
	Token t = ts.get();
	if (t.kind == print) t = ts.get();
	if (t.kind == quit) return;
	ts.putback(t);
	var = statement();
	cout << "= " << var << "\n";
	}
}

int main() {
	try {
		calculate();
		return 0;
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
