relational_expression1:
    exp LESS exp {printf("hellohello2");}
    | exp GREATER exp {printf("hellohello3");}
    | exp LE_OP exp {printf("hellohello4");}
    | exp GE_OP exp {printf("hellohello5");}
    | exp AND exp {printf("hellohello7");}
    ;

exp:
    additive_expression
    |multiplicative_expression

