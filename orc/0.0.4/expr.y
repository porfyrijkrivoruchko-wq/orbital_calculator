%{
#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <string>
#include "skymech.hpp"

#define YYSTYPE Real

int yylex();
void yyerror(const char* msg) {
  throw std::string(msg);
}

extern Real parse_result;

%}

%token UNIT CONST
%left '+' '-'
%left '*' '/'
%left NEG
%right '^'

%%

value
: expr UNIT { parse_result=$$=$1*$2; }
| value expr UNIT { parse_result=$$=$1+$2*$3; }
;

expr
: '-' expr %prec NEG { $$=-$2; }
| expr '+' expr { $$=$1+$3; }
| expr '-' expr { $$=$1-$3; }
| expr '*' expr { $$=$1*$3; }
| expr '/' expr { $$=$1/$3; }
| expr '^' expr { $$=pow($1,$3); }
| '(' expr ')' { $$=$2; }
| CONST { $$=$1; }
;

%%

extern const char* parse_buf;
extern int buf_ptr;
extern const Unit* expr_unit;

int yylex() {
  while(isspace(parse_buf[buf_ptr]))
    ++buf_ptr;
  if(isdigit(parse_buf[buf_ptr])) {
    int n=0;
    sscanf(parse_buf+buf_ptr,"%lg%n", &yylval, &n);
    buf_ptr+=n;
    return CONST;
  }
  auto u=(*expr_unit)[parse_buf+buf_ptr];
  if(u!=expr_unit->end())
  {
    yylval=(*u).second;
    buf_ptr+=(*u).first.size();
    return UNIT;
  }
  return parse_buf[buf_ptr++];
}
