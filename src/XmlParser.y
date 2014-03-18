
%{
/* ----------------------------------------------------------------------------- C/C++ prefix */
#include "XmlDocument.hpp"
#include "XmlParser.hpp"

int yylex(void);

void yyerror(Xml::Document ** e, const char * msg);


%}


/* ----------------------------------------------------------------------------- parse parameter */
%parse-param {Xml::Document ** e}


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
    : emptytag {}
    | stag content etag {}
    ;

emptytag
    : INF NOM atts SLASH SUP {}
    ;

stag
    : INF NOM atts SUP {}
    | INF NOM COLON NOM atts SUP {}
    ;

etag
    : INF SLASH NOM SUP {}
    ;

att
    : NOM EGAL VALEUR {}
    ;

atts
    : atts att {}
    | /* vide */ {}
    ;

item
    : element {}
    | DONNEES {}
    ;

content
    : content item {}
    | /* vide */ {}
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
yyerror(Xml::Document ** e, const char * msg)
{
    yyerror(msg);
}

extern FILE * yyin;

Xml::Document *
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

    Xml::Document * e = 0;

    {
        yyin = f;

        yyparse(&e);

        yyin = 0;
    }

    fclose(f);

    return e;
}
