%{

/* Declarations section */
#include <stdio.h>    
#include "node.hpp"
#include "hw3_output.hpp" 
#define YYSTYPE Node*  
#include "parser.tab.hpp" 




%}

%option yylineno
%option noyywrap

whitespace		([\r\t\n ])   
void			(void)
int				(int)
byte			(byte)
b				(b)
bool			(bool)
auto			(auto)
and				(and)
or				(or)
not				(not)
true			(true)
false			(false)
return			(return)
if				(if)
else			(else)
while			(while)
break			(break)
continue		(continue)
sc				(;)
comma			(,)
lparen			(\()
rparen			(\))
lbrace			(\{)
rbrace			(\})
assign			(=)
relop			(==|!=|<=|>=|<|>)
binop			(\+|-|\*|\/)
comment			(\/\/[^\n\r]*[\r|\n|\r\n]?)
id				([a-zA-Z][a-zA-Z0-9]*)
num				([1-9][0-9]*|0)
printable		(([ -!#-\[\]-~]|[	]|\\[\\|"|n|r|t|0]|\\x[0-7][0-9A-Fa-f])*)
notescapeseq	(\\[^n"rt0x\\]|\\x[^0-7].|\\x[0-7][^0-9A-Fa-f]|\\x)
string			(\"([^\n\r\"\\]|\\[rnt\"\\])+\")

%%

{void}						return VOID;
{int}						return INT;
{byte}						return BYTE;
{b}							return B;
{bool}						return BOOL;
{auto}						return AUTO;
{and}						return AND;
{or}						return OR;
{not}						return NOT;
{true}						return TRUE;
{false}						return FALSE;
{return}					return RETURN;
{if}						return IF;
{else}						return ELSE;
{while}						return WHILE;
{break}						return BREAK;
{continue}					return CONTINUE;
{sc}						return SC;
{comma}						return COMMA;
{lparen}					return LPAREN;
{rparen}					return RPAREN;
{lbrace}					return LBRACE;
{rbrace}					return RBRACE;
{assign}					return ASSIGN;
{relop}						return RELOP;
{binop}						return BINOP;
{comment}					;
{id}						{yylval = new myVariable(yytext);
                              return ID;}
{num}						{yylval = new myNumber(yytext);
                              return NUM;}
{string}					return STRING;
{whitespace}				;			
<<EOF>>						return 0;
.							output::errorLex(yylineno); exit(0);


%%

