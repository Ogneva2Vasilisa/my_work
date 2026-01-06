%{
#include "grammar.tab.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
%}

%option noyywrap
%option yylineno

/* Определения токенов */
IDENTIFIER    [a-zA-Z_][a-zA-Z0-9_]*
BINARY_NUMBER  0[bB][01]+
NUMBER        [0-9]+[uUlL]*
HEX_NUMBER    0[xX][0-9a-fA-F]+[uUlL]*
FLOAT         ([0-9]+(\.[0-9]*)?|\.[0-9]+)([eE][-+]?[0-9]+)?[fFlL]?
STRING        \"([^\\\"]|\\.)*\"
CHAR          \'([^\\\']|\\.)\'
COMMENT       \/\/[^\n]*|\/\*([^*]|\*+[^/*])*\*+\/

%%
"typedef"     { return TYPEDEF; }
"long"([ \t\r]+)"long"      { return LONG_LONG; }
"unsigned"([ \t\r]+)"long"([ \t\r]+)"long" { return UNSIGNED_LONG_LONG; }
"signed"([ \t\r]+)"long"([ \t\r]+)"long"   { return SIGNED_LONG_LONG; }
"unsigned"([ \t\r]+)"short"             { return UNSIGNED_SHORT; }
"signed"([ \t\r]+)"short"               { return SIGNED_SHORT; }
"unsigned"([ \t\r]+)"long"              { return UNSIGNED_LONG; }
"signed"([ \t\r]+)"long"                { return SIGNED_LONG; }
"unsigned"([ \t\r]+)"int"               { return UNSIGNED_TYPE; }
"signed"([ \t\r]+)"int"                 { return SIGNED_TYPE; }
"long"([ \t\r]+)"double"                { return LONG_DOUBLE; }

"static"      { return STATIC; }
"const"       { return STATIC; }
"if"          { return IF; }
"else"        { return ELSE; }
"do"          { return DO; }
"while"       { return WHILE; }
"for"         { return FOR; }
"return"      { return RETURN; }
"break"       { return BREAK; }
"switch"      { return SWITCH; }
"case"        { return CASE; }
"default"     { return DEFAULT; }
"int"         { return TYPE; }
"float"       { return TYPE; }
"char"        { return TYPE; }
"double"      { return TYPE; }
"void"        { return TYPE; }
"bool"        { return TYPE; }
"struct"      { return STRUCT; }
"union"       { return UNION; }
"enum"        { return ENUM; }
"long"        { return LONG; }
"short"       { return SHORT; }
"unsigned"    { return UNSIGNED; }
"signed"      { return SIGNED; }
"FILE"        { return FILE_TYPE; }
"sizeof"      { return SIZEOF; }
"NULL"        { return NULLVAL; }
"EOF"         { return EOFVAL; }
"true"        { return TRUE_VAL; }
"false"       { return FALSE_VAL; }

"++"          { return INCREMENT; }
"--"          { return DECREMENT; }
"<<"          { return LEFT_SHIFT; }
">>"          { return RIGHT_SHIFT; }
"+="          { return PLUS_ASSIGN; }
"-="          { return MINUS_ASSIGN; }
"*="          { return MULTIPLY_ASSIGN; }
"/="          { return DIVIDE_ASSIGN; }
"%="          { return MODULO_ASSIGN; }
"<<="         { return LEFT_SHIFT_ASSIGN; }
">>="         { return RIGHT_SHIFT_ASSIGN; }
"<="          { return LE_OP; }
">="          { return GE_OP; }
"=="          { return EQUAL; }
"!="          { return NEQUAL; }
"&&"          { return AND; }
"||"          { return OR; }
"->"          { return ARROW; }

"+"           { return PLUS; }
"-"           { return MINUS; }
"*"           { return MULTIPLY; }
"/"           { return DIVIDE; }
"%"           { return MODULO; }
"="           { return ASSIGN; }
"<"           { return LESS; }
">"           { return GREATER; }
"&"           { return AMPERSAND; }
"|"           { return PIPE; }
"^"           { return CARET; }
"!"           { return NOT; }
"."           { return DOT; }
";"           { return SEMICOLON; }
","           { return COMMA; }
":"           { return COLON; }
"("           { return LPAREN; }
")"           { return RPAREN; }
"{"           { return LBRACE; }
"}"           { return RBRACE; }
"["           { return LBRACKET; }
"]"           { return RBRACKET; }

{COMMENT}     { /* ignore comments */ }
[ \t\r]+      { /* ignore whitespace */ }
\n            { /* ignore newlines */ }

{BINARY_NUMBER} {
    yylval.num = (int)strtol(yytext + 2, NULL, 2);
    return NUMBER;
}

{HEX_NUMBER}  { 
                yylval.num = (int)strtol(yytext, NULL, 16); 
                return HEX_NUMBER; 
              }
{NUMBER}      { 
                yylval.num = atoi(yytext); 
                return NUMBER; 
              }
{FLOAT}       { 
                yylval.fnum = atof(yytext); 
                return FLOAT; 
              }
{STRING}      { 
                yylval.str = strdup(yytext); 
                return STRING; 
              }
{CHAR}        { 
                yylval.str = strdup(yytext); 
                return CHAR; 
              }
{IDENTIFIER}  { 
                yylval.str = strdup(yytext); 
                return IDENTIFIER; 
              }

.             { 
                fprintf(stderr, "Unexpected character: %c\n", *yytext); 
                return *yytext; 
              }
%%

