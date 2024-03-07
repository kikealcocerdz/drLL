// Grupo de trabajo 2. Enrique Alcocer Díaz, Iván Fernández Martin-Gil
// 100472118@alumnos.uc3m.es 100472263@alumnos.uc3m.es

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T_NUMBER 1001
#define T_OPERATOR 1002
#define T_VARIABLE 1003

int token;          // Here we store the current token/literal
int old_token = -1; // Sometimes we need to check the previous token
int number; // The value of the number
char variable;
int old_number = 0;
int token_val; // or the arithmetic operator
int old_token_val = -1;
// TO DO: Pack these variables in a struct

int line_counter = 1;

int ParseExpression() {
  int val;
  val = ParseTerm();
  while (token == '+' || token == '-') {
    if (token == '+') {
      MatchSymbol('+');
      val = val + ParseTerm();
    } else {
      MatchSymbol('-');
      val = val - ParseTerm();
    }
  }
  return val;
} // Prototype for forward reference

void update_old_token() { // Sometimes we need to check the previous token
                          // TO DO: Change to a more structured code
  old_token = token;
  old_number = number;
  old_token_val = token_val;
}

int rd_lex() {
  int c;

  do {
    c = getchar();
    if (c == '\n')
      line_counter++; // info for rd_syntax_error()
  } while (c == ' ' || c == '\t');

  if (isdigit(c)) {
    ungetc(c, stdin);
    update_old_token();
    scanf("%d", &number);
    token = T_NUMBER;
    return (token); // returns the Token for Number
  }

  if (c == '+' || c == '-' || c == '*' || c == '/') {
    update_old_token();
    token_val = c;
    token = T_OPERATOR;
    return (token);
  } // returns the Token for Arithmetic Operators

  update_old_token();
  token = c;
  return (token); // returns a literal
}

void rd_syntax_error(int expected, int token, char *output) {
  fprintf(stderr, "ERROR in line %d ", line_counter);
  fprintf(stderr, output, expected, token);

  exit(0);
}

void MatchSymbol(int expected_token) {
  if (token != expected_token) {
    rd_syntax_error(expected_token, token,
                    "token %d expected, but %d was read");
  }
  rd_lex();
}

#define ParseLParen()                                                          \
  MatchSymbol('('); // More concise and efficient definitions
#define ParseRParen()                                                          \
  MatchSymbol(')'); // rather than using functions
                    // This is only useful for matching Literals

int ParseNumber() // Parsing Non Terminals and some Tokens require more
{                 // complex functions
  MatchSymbol(T_NUMBER);
  return number;
}
int ParseVariable() // Parsing Non Terminals and some Tokens require more
{                 // complex functions
  MatchSymbol(T_VARIABLE);
  return variable;
}

int ParseAxiom() { // A::(R|N
  if (token ==T_NUMBER){
    ParseNumber();
    }else if(token == '('){
      ParseLParen();
      ParseRest();
    else{
      rd_syntax_error (token, 0, "Token %d was read, but a Number or a Left Parenthesis was expected");
    }
  
}
}

int ParseRest() { // R::= =VE)|OPP)
  int val;
  if (token == '='){
    MatchSymbol('=');
    ParseVariable();
    ParseExpression();
    ParseRParen();
  }
  else{
    switch (token){			// Expression_Rest derives in alternatives
		case '+' :  			// requires checking FIRST(ExpressionRest))
		case '-' :
		case '*' :
		case '/' :  
			    	break ;
		default :   rd_syntax_error (token, 0, "Token %d was read, but an Operator was expected");
			     	break ;
	}
	rd_lex () ;//Avanzamos el token
    ParseParameter();
    ParseParameter();
    ParseRParen();
  }
}
int ParseParameter(){ //P::=V|E
  if (token == T_VARIABLE){
    ParseVariable();
  }
  else if{
    ParseExpression();
  }else{
    rd_syntax_error (token, 0, "Token %d was read, but a Variable or an Expression was expected");
  }
  
}
int ParseExpression() { // E::= (OPP)|N
  if (token == T_NUMBER){
    ParseNumber();
  }
  else if (token == '('){
    ParseLParen();
    switch (token){			
		case '+' :  			
		case '-' :
		case '*' :
		case '/' :  
			    	break ;
		default :   rd_syntax_error (token, 0, "Token %d was read, but an Operator was expected");
			     	break ;
    rd_lex () ;//Avanzamos el token
    ParseParameter();
    ParseParameter();
    ParseRParen();
  }
}
}

int main(int argc, char **argv) {
  // Usage :  drLL -s  ==> evaluate a single Input Line
  //          drLL     ==> evalute multiple Input Lines until some error appears

  int flagMultiple = 1;

  if (argc >= 2) {
    if (strcmp("-s", argv[1]) == 0) {
      flagMultiple = 0;
    }
  }

  do {
    rd_lex();
    ParseAxion();
    printf("\n");
  } while (flagMultiple);

  system("PAUSE");
}
