%option yylineno
%option noyywrap

%{
#include "gram.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char current_line[1024];   // буфер для текущей строки
int line_pos = 0;
%}

%%


\n {
    current_line[line_pos] = '\0';   // завершили строку
    if (line_pos > 0) {
        line_pos = 0;                // сброс
        return END_LINE;             // есть содержимое → конец выражения
    } else {
        line_pos = 0;                // пустая строка или комментарий → пропускаем
    }
}


"+"                     { strcat(current_line, yytext); line_pos += strlen(yytext); return PLUS; }
"-"                     { strcat(current_line, yytext); line_pos += strlen(yytext); return MINUS; }
"*"                     { strcat(current_line, yytext); line_pos += strlen(yytext); return MULTIPLY; }
"/"                     { strcat(current_line, yytext); line_pos += strlen(yytext); return DIVIDE; }
"^"                     { strcat(current_line, yytext); line_pos += strlen(yytext); return POW; }
"("                     { strcat(current_line, yytext); line_pos += strlen(yytext); return LPAREN; }
")"                     { strcat(current_line, yytext); line_pos += strlen(yytext); return RPAREN; }
"print"                 { strcat(current_line, yytext); line_pos += strlen(yytext); return PRINT; }
"="                     { strcat(current_line, yytext); line_pos += strlen(yytext); return ASSIGN; }

"\$"[a-zA-Z0-9_]+   { strcat(current_line, yytext); line_pos += strlen(yytext);
                      yylval.str = strdup(yytext); return VAR; }

"\$"                { fprintf(stderr, "[LEX ERROR] Line %d: program variable name missing after '$'\n", yylineno);
                       }


[a-zA-Z_][a-zA-Z0-9_]* {strcat(current_line, yytext); line_pos += strlen(yytext); yylval.str = strdup(yytext); return IDENTIFIER; } /* обычные идентификаторы */
[0-9]+                  {strcat(current_line, yytext); line_pos += strlen(yytext); yylval.num = atoi(yytext); return NUMBER; }


"#".*                   { current_line[line_pos] = '\0'; /* комментарий игнорируем */ }

[ \t\r]+                { strcat(current_line, yytext); line_pos += strlen(yytext); }
.                       { strcat(current_line, yytext); line_pos += strlen(yytext); fprintf(stderr, "[ LEX ERROR] Line %d: unknown symbol '%s'\n", yylineno, yytext); }

%%