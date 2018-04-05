#include "std_lib_facilities.h"

// constructor {} initialization from 9.4, 9.7 
// this wos standard initialization used to get this working.

class Token {
public:
	Token(char ch)    
        :kind(ch), value(0) { }
    Token(char ch, double val)    
        :kind(ch), value(val) { }
    Token(char ch, string str)    
        :kind(ch), name(str) { }
	char kind;
	double value;
	string name;
};

class Token_stream {
public:
	Token_stream();
	Token get();
	void putback(Token t);
private:
	bool full = false;
	Token buffer;
};

class Variable {
public:
	Variable(string str)
		:name(str), value(0) {}
	Variable(string str, double val)
		:name(str), value(val) {}		
	string name;
	double value;
};

vector<Variable> var_table;

double get_value(string s); 
void set_value(string s, double d);

// Token get_token();
Token_stream ts;
const char number = '8';
const char quit = 'q';
const char print = ';';
const char name = 'a';
const char let = 'L';
const string declkey = "let";

double statement();
double declaration();
double expression();
double term();
double primary();