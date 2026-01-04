## Задача

Изучение основ языка **yacc, bison, flex**. Разработать программу, которая будет форматировать полиномы и считать возможные значения.

---
## Решение

Был написан лексер, который распознает все возможные символы, доступные для этого калькулятора полиномов:

```lex
%option yylineno
%option noyywrap
%{
#include "gram.tab.h"
#include <stdio.h>
%}

%%
"+"                     { return PLUS;      }
"-"                     { return MINUS;     }
"*"                     { return MULTIPLY;  }
"/"                     { return DIVIDE;    }
"^"                     { return POW;       }
"("                     { return LPAREN;    }
")"                     { return RPAREN;    }
"print"                 { return PRINT;     }
"="                     { return ASSIGN;    }
$[a-zA-Z0-9_]+          { yylval.str = strdup(yytext);  return VAR;         }
[0-9]+                  { yylval.num = atoi(yytext);    return NUMBER;      }
[a-zA-Z]               { yylval.str = strdup(yytext);  return IDENTIFIER;  }
\n                      { return END_LINE;    }
[ \t]+                  ;
.                       { fprintf(stderr, "[ERROR] Line %d: unknown symbol %s\n", yylineno, yytext); exit(1); }
%%
```


#### Сама логика приведения подобных членов реализована в файле gram.y
