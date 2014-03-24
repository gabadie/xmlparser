
%{
/* ----------------------------------------------------------------------------- C/C++ prefix */

#include <stdio.h>

#include "XmlParser.hpp"
#include "XmlText.hpp"
#include "XmlComment.hpp"
#include "XmlParserError.hpp"
#include "XmlParserInput.hpp"

int yylex(void);

void yyerror(void ** e, const char * msg);

%}


/* ----------------------------------------------------------------------------- parse parameter */
%parse-param {void ** e}


/* ----------------------------------------------------------------------------- C/C++ union */
%union{
    char * s;
    std::string * str;
    std::map<std::string, std::string> * attrs;
    std::list<Xml::Node *> * nodeList;
    Xml::Node * node;
    Xml::Element * e;
}


/* ----------------------------------------------------------------------------- tokens */

%token EGAL SLASH SUP SUPSPECIAL DOCTYPE COLON INFSPECIAL INF CDATABEGIN
%token <s> VALEUR DONNEES NOM CDATAEND COMMENT


/* ----------------------------------------------------------------------------- types */
%start document
%type <attrs> atts
%type <e> element
%type <e> emptytag
%type <e> stag
%type <nodeList> content
%type <nodeList> misc
%type <node> item
%type <node> miscitem
%type <str> etag


%%
/* ----------------------------------------------------------------------------- types rules */

document:
    misc element misc
    {
        /* ---------------------------------------------------- root */
        /*
         * Allocates the XML document
         */
        Xml::Document * doc = new Xml::Document();

        /*
         * Appends document nodes
         */
        if ($1 != nullptr)
        {
            for (auto node : *$1)
            {
                doc->appendNode(node);
            }

            /*
             * Delete document nodes' list allocated in rule 'misc'
             */
            delete $1;
        }

        /*
         * Appends document's root
         */
        if ($2 != nullptr)
        {
            doc->appendNode($2);
        }

        /*
         * Appends document nodes
         */
        if ($3 != nullptr)
        {
            for (auto node : *$3)
            {
                doc->appendNode(node);
            }

            /*
             * Delete document nodes' list allocated in rule 'misc'
             */
            delete $3;
        }

        /*
         * Returns the document to the yyparse()'s output parameter
         */
        *e = doc;
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

            $1->appendNode(node);
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

miscitem:
    COMMENT
    {
        /* ---------------------------------------------------- comment node */
        $$ = (Xml::Node *) new Xml::Comment(std::string($1));

        /*
         * $1 is char * allocated in XmlParser.lex with malloc(), then we free it.
         */
        free($1);
    };

misc:
    misc miscitem
    {
        /* ---------------------------------------------------- misc element */
        /*
         * we forward the misc list first.
         */
        $$ = $1;

        /*
         * miscitem parsing has successed.
         */
        if ($2 != nullptr)
        {
            /*
             * If no misc list yet, then we allocate it
             */
            if ($$ == nullptr)
            {
                $$ = new std::list<Xml::Node *>();
            }

            /*
             * We push the misc item at the tail of the misc list
             */
            $$->push_back($2);
        }
    } |
    /* vide */
    {
        /* ---------------------------------------------------- empty misc */
        /*
         * if misc is empty, we return an empty list.
         */
        $$ = nullptr;
    };

emptytag:
    INF NOM atts SLASH SUP
    {
        /* ---------------------------------------------------- empty element tag */
        /* TODO: empty tag */
        $$ = nullptr;
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
        $$ = nullptr;
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
        $$ = nullptr;
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
        $$ = (Xml::Node *) new Xml::Text(std::string($1));

        /*
         * $1 is char * allocated in XmlParser.lex with malloc(), then we free it.
         */
        free($1);
    } |
    miscitem
    {
        /* ---------------------------------------------------- misc element */
        $$ = $1;
    }

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

/*
 * Flex file number
 */
extern
int
yylineno;

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
        yylineno = 1;

        yyparse((void **) &e);
        yyrestart(stdin);

        if (!log)
        {
            std::cout << tmpLog;
        }
    }

    return e;
}
