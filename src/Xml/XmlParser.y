
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
%type <nodeList> content
%type <nodeList> misc
%type <node> doctype
%type <node> item
%type <node> miscitem
%type <node> node_processinstr
%type <str> etag
%type <str> doctype_decl


%%
/* ----------------------------------------------------------------------------- types rules */

document:
    misc node_element misc
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

node_element:
    INF NOM atts SLASH SUP
    {
        /* ---------------------------------------------------- empty element tag */
        $$ = new Xml::Element(std::string($2));

        for(auto const & p : *$3)
        {
            $$->setAttribute(p.first, p.second);
        }

        free($2);
        delete $3;
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
    stag content etag
    {
        /* ---------------------------------------------------- element with children */

        /*
         * Xml::Element is allocated in stag.
         */
        $$ = $1;

        for(Xml::Node * node : *$2)
        {
            if (node == nullptr)
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
        if ($1->tag() != *$3)
        {
            Xml::parserSemanticError("unexpected </" + *$3 + "> (it should have been </" + $1->name() + ">)");
        }

        /*
         * $3 has been created in content, then we delete it.
         */
        delete $3;
    } |
    stag content error
    {
        /* ---------------------------------------------------- element with children */

        /*
         * Xml::Element is allocated in stag.
         */
        $$ = $1;

        for(Xml::Node * node : *$2)
        {
            if (node == nullptr)
            {
                continue;
            }

            $1->appendNode(node);
        }

        /*
         * std::list<Xml::Node *> has been created in content, then we delete it.
         */
        delete $2;

        Xml::parserSyntaxError("missing closing element </" + $1->name() + ">");
        yyerrok;
    };

node_processinstr:
    INFSPECIAL NOM atts SUPSPECIAL
    {
        /* ------------------------------------------ processing instruction */
        Xml::ProcessingInstruction * xmlpi = new Xml::ProcessingInstruction(std::string($2), "", "");

        for(auto const & p : *$3)
        {
            xmlpi->setAttribute(p.first, p.second);
        }

        free($2);
        delete $3;
        $$ = xmlpi;
    };

miscitem:
    COMMENT
    {
        /* ---------------------------------------------------- comment node */
        if ($1[0] == 0)
        {
            $$ = nullptr;
        }
        else
        {
            $$ = static_cast<Xml::Node *>(new Xml::Comment(std::string($1)));
        }

        /*
         * $1 is char * allocated in XmlParser.lex with malloc(), then we free it.
         */
        free($1);
    } |
    node_processinstr
    {
        /* ------------------------------------------ processing instruction */
        $$ = $1;
    } |
    doctype
    {
        /* ------------------------------------------ doctype */
        $$ = $1;
    };

doctype:
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
    misc DONNEES
    {
        Xml::parserSyntaxError(std::string("Text found outside root: ") + "\"" + std::string($2) + "\"");
        $$ = nullptr;
        free($2);
    } |
    /* empty */
    {
        /* ---------------------------------------------------- empty misc */
        /*
         * if misc is empty, we return an empty list.
         */
        $$ = nullptr;
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

item:
    node_element
    {
        /* ---------------------------------------------------- element in another element */
        $$ = static_cast<Xml::Node *>($1);
    } |
    DONNEES
    {
        /* ---------------------------------------------------- text in an element */
        if ($1[0] == '\0')
        {
            $$ = nullptr;
        }
        else
        {
            $$ = static_cast<Xml::Node *>(new Xml::Text(std::string($1)));
        }

        /*
         * $1 is char * allocated in XmlParser.lex with malloc(), then we free it.
         */
        free($1);
    } |
    CDATABEGIN CDATAEND
    {
        /* ---------------------------------------------------- CDATA node */
        if ($2[0] == '\0')
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
    } |
    miscitem
    {
        /* ---------------------------------------------------- misc element */
        $$ = $1;
    };

content:
    content item
    {
        /* ---------------------------------------------------- element's content */
        $$ = $1;
        $$->push_back($2);
    } |
    /* empty */
    {
        /* ---------------------------------------------------- element's content end */
        $$ = new std::list<Xml::Node *>();
    };

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
