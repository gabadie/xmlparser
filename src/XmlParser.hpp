#ifndef _H_XML_PARSER
#define _H_XML_PARSER

#define YY_NO_UNPUT

#include <iostream>
#include <stdio.h>
#include <string>

/*
 * Includes the header generated by BISON
 */
#include "XmlParser.y.hpp"


#include "XmlDocument.hpp"


int yyerror(char const *s);
int yyerror(std::string const & s);


namespace Xml
{

    /*
     * Parse a XML file
     */
    Xml::Document *
    parse(std::string const & path);

}

#endif //_H_XML_PARSER
