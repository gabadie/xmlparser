
%{
/* ----------------------------------------------------------------------------- C/C++ prefix */

#include <stdio.h>

#include "XmlParser.hpp"
#include "XmlText.hpp"
#include "XmlParserError.hpp"
#include "XmlParserInput.hpp"

int yylex(void);

void yyerror(void ** e, const char * msg);

namespace Xml
{
    inline
    void
    appendNode(Element * element, Node * node)
    {
        element->appendNode(node);
    }

    inline
    Text *
    parserText(std::string const & content)
    {
        return new Xml::Text(content);
    }
}

%}


/* ----------------------------------------------------------------------------- parse parameter */
%parse-param {void ** e}


/* ----------------------------------------------------------------------------- C/C++ union */
%union{
    char * s;
    std::string * str;
    std::map<std::string, std::string> * attrs;
    std::list<Xml::Node *> * node_list;
    Xml::Node * node;
    Xml::Element * e;
}


/* ----------------------------------------------------------------------------- tokens */

%token EGAL SLASH SUP SUPSPECIAL DOCTYPE COLON INFSPECIAL INF CDATABEGIN
%token <s> VALEUR DONNEES COMMENT NOM CDATAEND


/* ----------------------------------------------------------------------------- types */
%start document
%type <e> element
%type <e> stag
%type <str> etag
%type <attrs> atts
%type <node_list> content
%type <node> item


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

        /*
         * Xml::Element is allocated in stag.
         */
        $$ = $1;

        for(Xml::Node * node : *$2)
        {
            if (node == 0)
            {
                continue;
            }

            Xml::appendNode($1, node);
        }

        /*
         * std::list<Xml::Node *> has been created in content, then we delete it.
         */
        delete $2;

        /*
         * Checks that the closing function is working
         */
        if ($1->name() != *$3)
        {
            Xml::parserSemanticError("unexpected </" + *$3 + "> (it should have been </" + $1->name() + ">)");

            delete $$;
            $$ = 0;
        }

        /*
         * $3 has been created in content, then we delete it.
         */
        delete $3;
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
        $$ = new Xml::Element(std::string($2));
    } |
    INF NOM COLON NOM atts SUP
    {
        /* ---------------------------------------------------- nonempty element start tag (with namespace) */
        /* TODO: namespaces */
    };

etag:
    INF SLASH NOM SUP
    {
        /* ---------------------------------------------------- nonempty element end tag */
        $$ = new std::string($3);

        /*
         * $3 is char * allocated in XmlParser.lex with malloc(), then we free it.
         */
        free($3);
    } |
    INF SLASH NOM COLON NOM SUP
    {
        /* ---------------------------------------------------- nonempty element end tag (with namespace) */
        /* TODO: namespaces */
    };

atts:
    atts NOM EGAL VALEUR
    {
        /* ---------------------------------------------------- element's attributes */
        $1->insert({std::string($2), std::string($4)});

        /*
         * $2 and $4 is char * allocated in XmlParser.lex with malloc(), then we free them.
         */
        free($2);
        free($4);
    } |
    /* vide */
    {
        /* ---------------------------------------------------- element with children */
        $$ = new Xml::Element::AttributesMap();
    };

item:
    element
    {
        /* ---------------------------------------------------- element in another element */
        $$ = (Xml::Node *) $1;
    } |
    DONNEES
    {
        /* ---------------------------------------------------- text in an element */
        $$ = (Xml::Node *) Xml::parserText(std::string($1));

        /*
         * $1 is char * allocated in XmlParser.lex with malloc(), then we free it.
         */
        free($1);
    };

content:
    content item
    {
        /* ---------------------------------------------------- element's content */
        $$ = $1;
        $$->push_back($2);
    } |
    /* vide */
    {
        /* ---------------------------------------------------- element's content end */
        $$ = new std::list<Xml::Node *>();
    };


%%
/* ----------------------------------------------------------------------------- C/C++ suffix */

void
yyerror(void ** e, const char * msg)
{
    Xml::parserSyntaxError(msg);
}

extern
int
yy_flex_debug;

void
yyrestart(FILE * input_file);

Xml::Document *
Xml::parse(std::istream & xmlContent, Xml::Log * log)
{
    Xml::Document * e = 0;

    {
        Xml::Log tmpLog;

        if (log)
        {
            Xml::parserBindLog(*log);
        }
        else
        {
            Xml::parserBindLog(tmpLog);
        }

        Xml::flexSetInput(xmlContent);

        yy_flex_debug = 0;
        yyparse((void **) &e);
        yyrestart(stdin);

        if (!log)
        {
            std::cout << tmpLog;
        }
    }

    return e;
}
