
%{
/* ----------------------------------------------------------------------------- C/C++ prefix */
#include "XmlDocument.hpp"
#include "XmlParser.hpp"

int yylex(void);

void yyerror(void ** e, const char * msg);


%}


/* ----------------------------------------------------------------------------- parse parameter */
%parse-param {void ** e}


/* ----------------------------------------------------------------------------- C/C++ union */
%union{
    std::string * string_val;
    char * s;
    Xml::Element * e;
}


/* ----------------------------------------------------------------------------- tokens */
%token <string_val> STRING_LITERAL

%token EGAL SLASH SUP SUPSPECIAL DOCTYPE COLON INFSPECIAL INF CDATABEGIN
%token <string_val> VALEUR DONNEES COMMENT NOM CDATAEND


/* ----------------------------------------------------------------------------- types */
%start document
%type <string_val> document
%type <e> element


%%
/* ----------------------------------------------------------------------------- types rules */

document:
    element
    {
        /* ---------------------------------------------------- root */
        *e = new Xml::Document($1);
    };

element:
    emptytag
    {
        /* ---------------------------------------------------- empty element */
        /* TODO */
    } |
    stag content etag
    {
        /* ---------------------------------------------------- element with children */

    };

emptytag:
    INF NOM atts SLASH SUP
    {
        /* ---------------------------------------------------- empty element tag */

    };

stag:
    INF NOM atts SUP
    {
        /* ---------------------------------------------------- nonempty element start tag */

    } |
    INF NOM COLON NOM atts SUP
    {
        /* ---------------------------------------------------- nonempty element start tag */

    };

etag:
    INF SLASH NOM SUP
    {
        /* ---------------------------------------------------- nonempty element end tag */

    };

att:
    NOM EGAL VALEUR
    {
        /* ---------------------------------------------------- element's attribute */

    };

atts:
    atts att
    {
        /* ---------------------------------------------------- element's attributes */

    } |
    /* vide */
    {
        /* ---------------------------------------------------- element with children */

    };

item:
    element
    {
        /* ---------------------------------------------------- element in another element */

    } |
    DONNEES
    {
        /* ---------------------------------------------------- text in an element */

    };

content:
    content item
    {
        /* ---------------------------------------------------- element's content */

    } |
    /* vide */
    {
        /* ---------------------------------------------------- element's content end */

    };


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
yyerror(void ** e, const char * msg)
{
    yyerror(msg);
}

extern FILE * yyin;

Xml::Document *
Xml::parse(char const * path)
{
    if (path == 0)
    {
        return nullptr;
    }

    FILE * f = fopen(path, "rb");

    if (f == 0)
    {
        return nullptr;
    }

    Xml::Document * e = 0;

    {
        yyin = f;

        yyparse((void **) &e);

        yyin = 0;
    }

    fclose(f);

    return e;
}
