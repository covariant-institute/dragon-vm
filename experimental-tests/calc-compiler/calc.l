%{
#include <stdio.h>
#include "y.tab.h"

int
yywrap(void)
{
    return 1;
}
%}

%%

"+"             return ADD;
"-"             return SUB;
"*"             return MUL;
"/"             return DIV;
"\n"            return CR;

([1-9][0-9]*)|0|([0-9]+\.[0-9]*) {
    float temp;
    sscanf(yytext, "%f", &temp);
    yylval.float_value = temp;
    return FLOAT_LITERAL;
}

[ \t] ;

. {
    fprintf(stderr, "lexical error.\n");
    exit(1);
}

%%

