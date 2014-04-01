
%{
/* ----------------------------------------------------------------------------- C/C++ prefix */

#include <stdio.h>

#include "XmlCharacterData.hpp"
#include "XmlComment.hpp"
#include "XmlDoctype.hpp"
#include "XmlParser.hpp"
#include "XmlParserError.hpp"
#include "XmlParserInput.hpp"
#include "XmlProcessingInstruction.hpp"
#include "XmlText.hpp"

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
%type <e> node_element
%type <e> stag
%type <nodeList> nodes
%type <node> node
%type <node> node_cdata
%type <node> node_doctype
%type <node> node_processinstr
%type <str> etag
%type <str> doctype_decl


%%
/* ----------------------------------------------------------------------------- types rules */

document:
    nodes
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
                if (Xml::Document::canAppend(node) == false)
                {
                    Xml::parserSyntaxError(std::string("Unexpected node"));
                    delete node;
                    continue;
                }

                doc->appendNode(node);
            }

            /*
             * Delete document nodes' list allocated in rule 'misc'
             */
            delete $1;
        }

        /*
         * Returns the document to the yyparse()'s output parameter
         */
        *e = doc;
    };

nodes:
    nodes node
    {
        /* ---------------------------------------------------- append node */
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
    /* empty */
    {
        /* ---------------------------------------------------- empty nodes */
        /*
         * if misc is empty, we return an empty list.
         */
        $$ = nullptr;
    };

node:
    node_element
    {
        /* ---------------------------------------------------- element in another element */
        $$ = static_cast<Xml::Node *>($1);
    } |
    node_doctype
    {
        $$ = $1;
    } |
    DONNEES
    {
        /* ---------------------------------------------------- text node */
        if ($1[0] == '\0')
        {
            $$ = nullptr;
        }
        else
        {
            $$ = static_cast<Xml::Node *>(new Xml::Text($1));
        }

        /*
         * $1 is char * allocated in XmlParser.lex with malloc(), then we free it.
         */
        free($1);
    } |
    COMMENT
    {
        /* ---------------------------------------------------- comment node */
        if ($1[0] == '\0')
        {
            $$ = nullptr;
        }
        else
        {
            $$ = static_cast<Xml::Node *>(new Xml::Comment($1));
        }

        /*
         * $1 is char * allocated in XmlParser.lex with malloc(), then we free it.
         */
        free($1);
    } |
    node_cdata
    {
        $$ = $1;
    } |
    node_processinstr
    {
        $$ = $1;
    };

node_element:
    INF NOM atts SLASH SUP
    {
        /* ---------------------------------------------------- empty element tag */
        $$ = new Xml::Element(std::string($2));

        for(auto const & p : *$3)
        {
            $$->setAttribute(p.first, p.second);
        }

        delete $3;

        /*
         * $1 is char * allocated in XmlParser.lex with malloc(), then we free it.
         */
        free($2);
    } |
    INF NOM COLON NOM atts SLASH SUP
    {
        /* ---------------------------------------------------- empty element tag with namespace*/
        $$ = new Xml::Element(std::string($4), std::string($2));

        for(auto const & p : *$5)
        {
            $$->setAttribute(p.first, p.second);
        }

        free($2);
        free($4);
        delete $5;
    } |
    stag nodes etag
    {
        /* ---------------------------------------------------- element with children */

        /*
         * Xml::Element is allocated in stag.
         */
        $$ = $1;

        /*
         * Appends element's nodes
         */
        if ($2 != nullptr)
        {
            for (auto node : *$2)
            {
                if (Xml::Element::canAppend(node) == false)
                {
                    Xml::parserSyntaxError(std::string("Unexpected node"));
                    delete node;
                    continue;
                }

                $1->appendNode(node);
            }

            /*
             * Delete document nodes' list allocated in rule 'misc'
             */
            delete $2;
        }

        /*
         * Checks that the closing function is working
         */
        if ($1->tag() != *$3)
        {
            Xml::parserSemanticError("unexpected </" + *$3 + "> (it should have been </" + $1->name() + ">)");
        }

        /*
         * $3 has been created in content, then we delete it.
         */
        delete $3;
    } |
    stag nodes error
    {
        /* ---------------------------------------------------- element with children */

        /*
         * Xml::Element is allocated in stag.
         */
        $$ = $1;

        /*
         * Appends element's nodes
         */
        if ($2 != nullptr)
        {
            for (auto node : *$2)
            {
                if (Xml::Element::canAppend(node) == false)
                {
                    Xml::parserSyntaxError(std::string("Unexpected node"));
                    delete node;
                    continue;
                }

                $1->appendNode(node);
            }

            /*
             * Delete document nodes' list allocated in rule 'misc'
             */
            delete $2;
        }

        Xml::parserSyntaxError("missing closing element </" + $1->name() + ">");
        yyerrok;
    };

node_processinstr:
    INFSPECIAL NOM atts SUPSPECIAL
    {
        /* ------------------------------------------ processing instruction */
        auto xmlpi = new Xml::ProcessingInstruction(std::string($2), "", "");

        for(auto const & p : *$3)
        {
            xmlpi->setAttribute(p.first, p.second);
        }

        $$ = xmlpi;

        delete $3;

        /*
         * $2 is char * allocated in XmlParser.lex with malloc(), then we free it.
         */
        free($2);
    };

node_cdata:
    CDATABEGIN CDATAEND
    {
        /* ---------------------------------------------------- CDATA node */
        if ($2[0] == 0)
        {
            $$ = nullptr;
        }
        else
        {
            $$ = static_cast<Xml::Node *>(new Xml::CharacterData(std::string($2)));
        }

        /*
         * $2 is char * allocated in XmlParser.lex with malloc(), then we free it.
         */
        free($2);
    } |
    CDATABEGIN error
    {
        /* ---------------------------------------------------- CDATA error */
        $$ = nullptr;
    };

node_doctype:
    /* ---------------------------------------------------- doctype node */
    DOCTYPE NOM SUP
    {
        std::cerr << "Parsing 1 " << $2 << std::endl;
        $$ = static_cast<Xml::Node *>(new Xml::Doctype(std::string($2)));
        free($2);
        std::cerr << "Parsing 1 END" << std::endl;
    } |
    DOCTYPE NOM doctype_decl SUP
    {
        std::cerr << "Parsing 2 " << $2 << " | " << $3 << std::endl;
        if($3 != nullptr)
        {
            $$ = static_cast<Xml::Node *>(new Xml::Doctype(std::string($2) + " " + std::string(*$3)));
            delete $3;
        }
        else
        {
            $$ = nullptr;
        }

        /*
         * $2 is char * allocated in XmlParser.lex with malloc(), then we free it.
         */
        free($2);
    } |
    DOCTYPE error
    {
        std::cerr << "Error" << std::endl;
        $$ = nullptr;
        Xml::parserSemanticError("Wrong doctype declaration");
    };

doctype_decl:
    NOM VALEUR
    {
        std::cerr << "Parsing " << $1 << " | " << $2 << std::endl;
        $$ = new std::string(std::string($1) + " \"" + std::string($2) + "\"");
        free($1);
        free($2);
    } |
    NOM VALEUR VALEUR
    {
        $$ = new std::string(std::string($1) + " \"" + std::string($2) + "\" \"" + std::string($3) + "\"");
        free($1);
        free($2);
        free($3);
    } |
    error
    {
        $$ = nullptr;
        Xml::parserSemanticError("Wrong doctype declaration");
    };

stag:
    INF NOM atts SUP
    {
        /* ---------------------------------------------------- nonempty element start tag */
        $$ = new Xml::Element(std::string($2));

        for(auto const & p : *$3)
        {
            $$->setAttribute(p.first, p.second);
        }

        free($2);
        delete $3;
    } |
    INF NOM COLON NOM atts SUP
    {
        /* ---------------------------------------------------- nonempty element start tag (with namespace) */
        $$ = new Xml::Element(std::string($4), std::string($2));

        for(auto const & p : *$5)
        {
            $$->setAttribute(p.first, p.second);
        }

        free($2);
        free($4);
        delete $5;
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
        auto element = new Xml::Element(std::string($5), std::string($3));
        $$ = new std::string(element->tag());

        free($3);
        free($5);
        delete element;
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
    atts NOM COLON NOM EGAL VALEUR
    {
        /* ---------------------------------------------------- element's attributes with namespace */
        std::string attrName = std::string($2) + ":" + std::string($4);
        $1->insert({attrName, std::string($6)});

        /*
         * $2, $4 and $6 is char * allocated in XmlParser.lex with malloc(), then we free them.
         */
        free($2);
        free($4);
        free($6);
    } |
    /* empty */
    {
        $$ = new Xml::Element::AttributesMap();
    } |

    /* Handle Xml::Element's attributes parsing errors */

    /* Shift/reduce warning caused by the rules "atts NOM EGAL" et "atts VALEUR"
     This conflict is safe because Bison is greedy and will match "atts NOM EGAL VALEUR" first
     instead of "atts NOM EGAL" and "atts VALEUR"
     A test in testXmlElementParsing checks this assertion. */
    atts NOM EGAL
    {
        Xml::parserSyntaxError(std::string("Ill-formed attribute: ") + "\"" + std::string($2) + "=\"");
        $$ = $1;
        free($2);
    } |
    atts EGAL VALEUR
    {
        Xml::parserSyntaxError(std::string("Ill-formed attribute: ") + "\"=" + std::string($3) + "\"");
        $$ = $1;
        free($3);
    } |
    atts VALEUR
    {
        Xml::parserSyntaxError(std::string("Ill-formed attribute: ") + "\"" + std::string($2) + "\"");
        $$ = $1;
        free($2);
    };

    /* Errors not handled */
    /*
    atts NOM
    {
        $$ = $1;
    } |
    atts NOM EGAL NOM
    {
        $$ = $1;
    }; /**/


%%
/* ----------------------------------------------------------------------------- C/C++ suffix */

void
yyerror(void **, const char * msg)
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
    Xml::Document * e = nullptr;

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
