
%{
/* ----------------------------------------------------------------------------- C/C++ prefix */

#include "XmlParser.hpp"


%}


/* ----------------------------------------------------------------------------- Flex configuration */
/* we don't have yywrap() */
%option noyywrap


/* ----------------------------------------------------------------------------- Terminals' definitions */

char        [a-zA-Z]
string      {char}+

/* ----------------------------------------------------------------------------- Terminals' rules */
%%

{string} {
    yylval.string_val = new std::string(yytext);
    return STRING_LITERAL;
}

[\n] {}

. {
    std::cerr << "SCANNER ";
    yyerror("");
    exit(1);
}
