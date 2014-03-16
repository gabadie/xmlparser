
%{
/* ----------------------------------------------------------------------------- C/C++ prefix */

#include "XmlParser.hpp"


%}
/* ----------------------------------------------------------------------------- Terminals' definitions */

char        [a-zA-Z]
string      [char]+


/* ----------------------------------------------------------------------------- Terminals' rules */
%%

{string}	{
    yylval.string_val = new std::string(yytext);
    return STRING_LITERAL;
}

. {
    std::cerr << "SCANNER ";
    yyerror("");
    exit(1);
}
