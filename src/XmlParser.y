
%{
/* ----------------------------------------------------------------------------- C/C++ prefix */

#include "XmlParser.hpp"

int yylex(void);

void yyerror(std::string ** e, const char * msg);


%}


/* ----------------------------------------------------------------------------- parse parameter */
%parse-param {std::string ** e}


/* ----------------------------------------------------------------------------- C/C++ union */
%union{
    std::string * string_val;
}


/* ----------------------------------------------------------------------------- tokens */
%token <string_val> STRING_LITERAL


/* ----------------------------------------------------------------------------- types */
%start main
%type <string_val> main


%%
/* ----------------------------------------------------------------------------- types rules */

main : STRING_LITERAL { *e = $1; }
     ;


%%
/* ----------------------------------------------------------------------------- C/C++ suffix */

int yyerror(std::string const & s)
{
    extern int yylineno;	// defined and maintained in lex.c
    extern char *yytext;	// defined and maintained in lex.c

    std::cerr << "ERROR: " << s << " at symbol \"" << yytext;
    std::cerr << "\" on line " << yylineno << std::endl;

    exit(1);
}

int yyerror(char const *s)
{
    return yyerror(std::string(s));
}

void yyerror(std::string ** e, const char * msg)
{
    yyerror(msg);
}
