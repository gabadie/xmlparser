#ifndef _H_XML_PARSER
#define _H_XML_PARSER

#define YY_NO_UNPUT

#include <iostream>
#include <stdio.h>
#include <string>

#include "XmlParser.y.hpp"

int yyerror(char const *s);
int yyerror(std::string const & s);

#endif //_H_XML_PARSER
