
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
    char * s;
}


/* ----------------------------------------------------------------------------- tokens */
%token <string_val> STRING_LITERAL

%token EGAL SLASH SUP SUPSPECIAL DOCTYPE COLON INFSPECIAL INF CDATABEGIN
%token <string_val> VALEUR DONNEES COMMENT NOM CDATAEND


/* ----------------------------------------------------------------------------- types */
%start document
%type <string_val> document


%%
/* ----------------------------------------------------------------------------- types rules */

document
    : element { *e = 0; }
    ;

element
    : INF NOM SUP content INF SLASH NOM SUP
    ;

content
    : content element
    | /* vide */
    ;


%%
/* ----------------------------------------------------------------------------- C/C++ suffix */

int
yyerror(std::string const & s)
{
    extern int yylineno;	// defined and maintained in lex.c
    extern char *yytext;	// defined and maintained in lex.c

    std::cerr << "ERROR: " << s << " at symbol \"" << yytext;
    std::cerr << "\" on line " << yylineno << std::endl;

    exit(3);
}

int
yyerror(char const *s)
{
    return yyerror(std::string(s));
}

void
yyerror(std::string ** e, const char * msg)
{
    yyerror(msg);
}

extern FILE * yyin;

std::string *
Xml::parse(std::string const & path)
{
    char const * sPath = path.c_str();

    if (sPath == 0)
    {
        return nullptr;
    }

    FILE * f = fopen(sPath, "rb");

    if (f == 0)
    {
        return nullptr;
    }

    std::string * e = 0;

    {
        yyin = f;

        yyparse(&e);

        yyin = 0;
    }

    fclose(f);

    return e;
}
