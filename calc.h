class Token {
public:
	Token(char ch)    
        :kind(ch), value(0) { }
    Token(char ch, double val)    
        :kind(ch), value(val) { }
	char kind;
	double value;
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

// Token get_token();
Token_stream ts;

double expression();
double term();
double primary();