%{
    using namespace std;
    #include "hw3_symbol_table.hpp"
    #include "hw3_enum_list.hpp"
    #include "hw3_expression.hpp"
    #include "hw3_relop_api.hpp"
    #include "hw3_master.hpp"
    #include "hw3_output.hpp"
    #include "parser.tab.hpp"
    #define YYSTYPE Master*
    #include <stdio.h>
	#include <iostream>
    void printLexError();
    void output::errorLex(int lineno);
%}

%option noyywrap
%option yylineno
%option nounput

whitespace	                                        ([ \t\r\n])
noZeroDigit	                                        ([1-9])
digit		                                        ([0-9])


%%
void					                            return VOID;
int                                                 return INT;
byte                                                return BYTE;
b                                                   return B;
bool                                                return BOOL;
auto												return AUTO;
and                                                 return AND;
or                                                  return OR;
not                                                 return NOT;
true                                                return TRUE;
false                                               return FALSE;
return                                              return RETURN;
if                                                  return IF;
else                                                return ELSE;
while                                               return WHILE;
break                                               return BREAK;
continue                                            return CONTINUE;
;                                                   return SC;
,                                                   return COMMA;
\(                                                  return LPAREN;
\)                                                  return RPAREN;
\{                                                  return LBRACE;
\}                                                  return RBRACE;
=                                                   return ASSIGN;
==|!=|\<=|\>=|\<|\> 		                        return RELOP;
\+|\-                                               return BINOP_ADDATIVE;
\*|\/                          	                    return BINOP_MULTIPLY;
[a-zA-Z][a-zA-Z0-9]*                                return ID;
{noZeroDigit}{digit}*                               return NUM;
0                                                   return NUM;
\"([^\n\r\"\\]|\\[rnt"\\])+\"                       return STRING;
\/\/[^\r\n]*(\r|\n|\r\n)?                           ;
{whitespace}                                        ;
.                                                   printLexError();
%%


void printLexError(){
    output::errorLex(yylineno);
    exit(0);
}