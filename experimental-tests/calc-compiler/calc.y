%{
#include <stdio.h>
#include <stdlib.h>

extern void do_init();
extern void do_add();
extern void do_sub();
extern void do_mul();
extern void do_div();
extern void do_number(float number);
extern void do_line_break();
extern void do_finish();

%}

%union {
    int          int_value;
    float        float_value;
}

%token <float_value>      FLOAT_LITERAL
%token ADD SUB MUL DIV CR
%type <float_value> expression term primary_expression

%%
line_list
    : line
    | line_list line
    ;

line
    : expression CR
    {
        do_line_break();
    }
expression
    : term {
    }
    | expression ADD term
    {
        do_add();
    }
    | expression SUB term
    {
        do_sub();
    }
    ;
term
    : primary_expression {
        do_number($1);
    }
    | term MUL primary_expression
    {
        do_number($3);
        do_mul();
    }
    | term DIV primary_expression
    {
        do_number($3);
        do_div();
    }
    ;
primary_expression
    : FLOAT_LITERAL
    ;
%%
int
yyerror(char const *str)
{
    extern char *yytext;
    fprintf(stderr, "parser error near %s\n", yytext);
    return 0;
}

int main(void)
{
    extern int yyparse(void);
    extern FILE *yyin;

    yyin = stdin;

    do_init();
    if (yyparse()) {
        fprintf(stderr, "Error ! Error ! Error !\n");
        exit(1);
    }
    do_finish();
}


