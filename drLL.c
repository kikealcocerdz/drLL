// Grupo de trabajo 2. Enrique Alcocer Díaz, Iván Fernández Martin-Gil
// 100472118@alumnos.uc3m.es 100472263@alumnos.uc3m.es


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T_NUMBER 1001
#define T_OPERATOR 1002
#define T_VARIABLE 1003

typedef struct {
  int token;
  int old_token;
  int number;
  char variable;
  int old_number;
  int token_val;
  int old_token_val;
} ParserData;

ParserData parser_data; // Declaramos la estructura de datos del parser

int ParseNumber();
char ParseVariable();
int ParseOperator();
int ParseParameter();
int ParseParameterRest(char paramop);
int ParseExpression();
int ParseSentencia();
int ParseAxiom();
int ParseRest();
char ask_variables();

int line_counter = 1;

void update_old_token() {
  parser_data.old_token = parser_data.token;
  parser_data.old_number = parser_data.number;
  parser_data.old_token_val = parser_data.token_val;
}

int rd_lex() {
  int c;

  do {
    c = getchar();
    if (c == '\n')
      line_counter++;
  } while (c == ' ' || c == '\t');

  if (isdigit(c)) {
    ungetc(c, stdin);
    update_old_token();
    scanf("%d", &parser_data.number);
    parser_data.token = T_NUMBER;
    return (parser_data.token);
  }

  if (isalpha(c)) {
    ungetc(c, stdin);
    update_old_token();
    scanf("%c", &parser_data.variable);
    parser_data.token = T_VARIABLE;
    return (parser_data.token);
  }

  if (c == '+' || c == '-' || c == '*' || c == '/') {
    update_old_token();
    parser_data.token_val = c;
    parser_data.token = T_OPERATOR;
    return (parser_data.token);
  }

  update_old_token();
  parser_data.token = c;
  return (parser_data.token);
}

void rd_syntax_error(int expected, int token, char *output) {
  fprintf(stderr, "ERROR in line %d ", line_counter);
  fprintf(stderr, output, expected, token);

  exit(0);
}

void MatchSymbol(int expected_token) {
  if (parser_data.token != expected_token) {
    rd_syntax_error(expected_token, parser_data.token,
                    "token %d expected, but %d was read in match_symbol\n");
  }
  rd_lex();
}

#define ParseLParen() MatchSymbol('(');
#define ParseRParen() MatchSymbol(')');

int ParseNumber() { //N::= 0|1|2| ... | 9999
  int aux = parser_data.number;
  printf("%d ", aux);
  MatchSymbol(T_NUMBER);
  return aux;
}

char ParseVariable() { //V::= A|B|C|...|Z
  char varaux = parser_data.variable;
  MatchSymbol(T_VARIABLE);
  return varaux;
}

int ParseOperator() { // O::= +|-|*|/
  MatchSymbol(T_OPERATOR);
  return parser_data.token_val;
}

int ParseParameter() { //P::= V|E
  if (parser_data.token == T_VARIABLE) {
    char paramVarAux = ParseVariable();
    printf("%c ", paramVarAux);
    printf("@ ");
  } else if (parser_data.token == T_NUMBER || parser_data.token == '(') {
    ParseExpression();
  } else {
    rd_syntax_error(parser_data.token, 0, "Token %d was read, but a Variable or an Expression was expected FALLO DE PARAMETER \n");
  }
}

int ParseParameterRest(char paramop) { // W::= PW| λ
  if (parser_data.token == T_VARIABLE || parser_data.token == T_NUMBER || parser_data.token == '(') {
    char paramVarAux = paramop;
    ParseParameter();
    printf("%c ", paramVarAux);
    ParseParameterRest(parser_data.token_val);
  } else {
    return 0;
  }
};

int ParseExpression() { // E::= (O P W)|N
  if (parser_data.token == T_NUMBER) {
    ParseNumber();
  } else if (parser_data.token == '(') {
    ParseLParen();
    char aux= parser_data.token_val;
    ParseOperator();
    ParseParameter();
    ParseParameterRest(aux);
    ParseRParen();
  }
}

int ParseSentencia() { // S::= (R)
  if (parser_data.token == '(') {
    ParseLParen();
    ParseRest();
    ParseRParen();
  }
}

int ParseAxiom() { // A::= S|N|V
  if (parser_data.token == T_NUMBER) {
    ParseNumber();
  } else if (parser_data.token == '(') {
    ParseSentencia();
  } else if (parser_data.token == T_VARIABLE) {
    char paramVarAux = ParseVariable();
    printf("%c ", paramVarAux);
    printf("@ ");
  } else {
    rd_syntax_error(parser_data.token, 0, "Token %d was read, but a Number or a Left Parenthesis was expected \n");
  }
}

int ParseRest() { // R::= =VS|OPW
  if (parser_data.token == '=') {
    MatchSymbol('=');
    char auxVar = ParseVariable();
    ParseAxiom();
    printf("dup %c ", auxVar);
    printf("! ");
  } else {
    char aux=ParseOperator();
    ParseParameter();
    ParseParameterRest(aux);
  }
}

int main(int argc, char **argv) {
  int flagMultiple = 1;
  if (argc >= 2) {
    if (strcmp("-s", argv[1]) == 0) {
      flagMultiple = 0;
    }
  }

  char alfabeto[26];
  for (int i = 0; i < 26; i++) {
    alfabeto[i] = 'A' + i;
    printf("VARIABLE %c \n", alfabeto[i]); 
  }

  do {
    rd_lex();
    ParseAxiom();
    printf(".");
    printf("\n");
    fflush(stdout);
  } while (flagMultiple);
  system("PAUSE");
}
