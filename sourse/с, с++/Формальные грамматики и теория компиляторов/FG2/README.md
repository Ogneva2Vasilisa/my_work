## Задача

**Научиться проектировать и реализовывать собственный формальный язык.**

---
## Требования и принципы языка

1. Продуманный синтаксис. Для максимального удобства синтаксис должен быть максимально похож на математический. Например, операция неявного умножения.

2. Операции с полиномами: бинарное и унарное сложение и вычитание (+,-), умножение (*), полином в степени  (^), деление (/).

3. Поддержка полиномов от разных переменных.

4. Поддержка переменных, которым можно присваивать полиномы.

5. Поддержка комментариев.

6. Развернутые сообщения об ошибках с указанием номера строки, где они произошли. Как минимум, должны появиться по 2-3 сообщения для каждого типа возможных ошибок:

	а) лексические. Определяются на этапе лексического анализа (lex).
	
	б) синтаксические. Определяются на этапе синтаксического анализа (yacc).
	
	в) семантические. Определяются на этапе исполнения.

---
## Решение

Отдельные интересные правила, которые стоит отметить:

- ```term factor``` — неявное умножение.
- ```factor POW factor``` — правило ассоциативности степени: взятие степени реализовано рекурсивно, плюс %right POW чтобы 2^3^2 парсилось справа налево как 2 ^( 3^2).
- ```MINUS expr %prec NEG``` — отдельный приоритет для унарного минуса (NEG), чтобы не было конфликтов с бинарным -.

**Упрощенная схема грамматики:**

```bison
%left PLUS MINUS
%left MULTIPLY DIVIDE
%left NEG
**%right POW**
program:
    | program line END_LINE
    ;

line:
    expr { … }
    | VAR ASSIGN expr { … }
    | IDENTIFIER ASSIGN NUMBER {…}
    | PRINT expr {…}
    | error {"[SYN ERROR]"}
    ;

expr:
    expr PLUS term { … }
    | expr MINUS term  { … }
    | **MINUS expr %prec NEG** { … }
    | term  { … }
    ;

term:
    term MULTIPLY factor { }
    | **term factor**         { … }
    | term DIVIDE factor { … }
    | factor             { … }
    ;

factor:
    factor POW factor { … } 
    | LPAREN expr RPAREN { … }
    | NUMBER             { … }
    | IDENTIFIER         { … }
    | VAR                { … }
    ;
```


**Представление полиномов и реализация операций. Структуры данных:**

- Моном (```Monomial```) — коэффициент + массив переменных с их степенями.
- Полином (```Polynomial```) — массив мономов, а также возможная структура для возведения в степень (рекурсивная форма).
- Переменные программы (```ProgramVar```) — связывают имя ($A) с конкретным многочленом.

---
## Тестирование программы и разбор типов ошибок

| ТЕСТ                                                       | ОТВЕТ                                                        |
| ---------------------------------------------------------- | ------------------------------------------------------------ |
| **# 1. Базовая арифметика**                                |                                                              |
| 2 + 3                                                      | 5                                                            |
| -1                                                         | -1                                                           |
| 4*5                                                        | 20                                                           |
| (4 * 5) -2                                                 | 18                                                           |
| 2^3                                                        | 8                                                            |
| -x                                                         | -x                                                           |
| -3x                                                        | -3x                                                          |
| 3x-2x                                                      | x                                                            |
| -2x^3                                                      | -2x^3                                                        |
| **# 2. Деление (валидное и ошибка)**                       |                                                              |
| 7/3                                                        | Remainder: 1                                                 |
|                                                            | 2                                                            |
| 7/0                                                        | [ERROR] Line 15: division by zero!                           |
|                                                            | 0                                                            |
| (2 * x ^ 2 + 3 * x + 1) / (x + 1)                          | Remainder: 1-x                                               |
|                                                            | 2x+x                                                         |
| (2 * x ^ 2 + 3 * x + 1) / (x)                              | Remainder: 1                                                 |
|                                                            | 2x+3x                                                        |
| **# 3. Полиномы от разных переменных**                     |                                                              |
| x + y                                                      | x+y                                                          |
| (2x+1)*(y-x)                                               | 2xy-2x^2+y-x                                                 |
| 3x^4 - 2x^3 + 5x - 1                                       | 3x^4-2x^3+5x-1                                               |
| 2(x + 1)^2                                                 | 2x^2+4x+2                                                    |
| (x + 1)^2                                                  | x^2+2x+1                                                     |
| **# 4. Одинаковые члены и упрощения**                      |                                                              |
| 2x * 3x                                                    | 6x^2                                                         |
| 2x^2 + 3x^2                                                | 5x^2                                                         |
| (2x^2 + 3x - 1) + (x^5 - 7)                                | 2x^2+3x-8+x^5                                                |
| (2x^2 + 3x - 1) - (x - 7)                                  | 2x^2+2x+6                                                    |
| **# 5. Присваивания и использование $A**                   |                                                              |
| $A = x + 1                                                 | $A = x + 1                                                   |
| $B = (2x + 1) * (3x + 2)                                   | $B = (2x + 1) * (3x + 2)                                     |
| $C = $A^2                                                  | $C = $A^2                                                    |
| $A + $B                                                    | 8x+3+6x^2                                                    |
| $C + 2                                                     | x^2+2x+3                                                     |
| print $C                                                   | print ~ x^2+2x+1                                             |
| **# 7. Лексические ошибки**                                |                                                              |
| 2 && 3        # неверный символ &                          | [ LEX ERROR] Line 41: unknown symbol '&'                     |
|                                                            | [ LEX ERROR] Line 41: unknown symbol '&'                     |
|                                                            | 6                                                            |
| @x + 1        # неверный символ @                          | [ LEX ERROR] Line 42: unknown symbol '@'                     |
|                                                            | x + 1                                                        |
| 5x$           # лишний символ $                            | [LEX ERROR] Line 43: program variable name missing after '$' |
|                                                            | 5x                                                           |
| **# 8. Синтаксические ошибки**                             |                                                              |
| (2+3          # незакрытая скобка                          | [SYN ERROR] Line 47: syntax error (token: '                  |
|                                                            | ')                                                           |
|                                                            | [SYN ERROR] Line 47: invalid expression near '               |
|                                                            | '                                                            |
| x + * y       # неправильная последовательность операторов | [SYN ERROR] Line 47: syntax error (token: 'x')               |
|                                                            | [SYN ERROR] Line 47: invalid expression near 'x'             |
|                                                            | [SYN ERROR] Line 47: syntax error (token: '+')               |
|                                                            | [SYN ERROR] Line 47: invalid expression near '+'             |
|                                                            | [SYN ERROR] Line 47: syntax error (token: '*')               |
|                                                            | [SYN ERROR] Line 47: invalid expression near '*'             |
|                                                            | [SYN ERROR] Line 47: syntax error (token: 'y')               |
|                                                            | [SYN ERROR] Line 47: invalid expression near 'y'             |
| /3 + 2        # выражение не может начинаться с /          | [SYN ERROR] Line 48: syntax error (token: '/')               |
|                                                            | [SYN ERROR] Line 48: invalid expression near '/'             |
|                                                            | [SYN ERROR] Line 48: syntax error (token: '3')               |
|                                                            | [SYN ERROR] Line 48: invalid expression near '3'             |
|                                                            | [SYN ERROR] Line 48: syntax error (token: '+')               |
|                                                            | [SYN ERROR] Line 48: invalid expression near '+'             |
|                                                            | [SYN ERROR] Line 48: syntax error (token: '2')               |
|                                                            | [SYN ERROR] Line 48: invalid expression near '2'             |
| x ++ x                                                     | [SYN ERROR] Line 49: syntax error (token: '+')               |
|                                                            | [SYN ERROR] Line 49: invalid expression near '+'             |
|                                                            | [SYN ERROR] Line 49: syntax error (token: 'x')               |
|                                                            | [SYN ERROR] Line 49: invalid expression near 'x'             |
| **# 9. Семантические ошибки**                              |                                                              |
| $X + 2        # переменная $X не определена                | [SYN ERROR] Line 52: Program variable $X is not initialized! |
| 2                                                          |                                                              |
| x^-1          # отрицательная степень                      | [SYN ERROR] Line 53: syntax error (token: '-')               |
|                                                            | [SYN ERROR] Line 53: invalid expression near '-'             |
|                                                            | [SYN ERROR] Line 53: syntax error (token: '1')               |
|                                                            | [SYN ERROR] Line 53: invalid expression near '1'             |
| y / 0         # деление на 0                               | [ERROR] Line 54: division by zero!                           |
|                                                            | 0                                                            |
| **# 10. Полином в степени полинома**                       |                                                              |
| x^x                                                        | (x)^( x)                                                     |
| 2^x                                                        | (2)^( x)                                                     |
| 2^(x+1)                                                    | (2)^( x+1)                                                   |
| (2^x)*(2^x)                                                | (2)^( 2x)                                                    |
| (x^x)*(x^3)                                                | (x)^( x+3)                                                   |
| x^x * x^2                                                  | (x)^( x+2)                                                   |
