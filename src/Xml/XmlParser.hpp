#ifndef _H_XML_PARSER
#define _H_XML_PARSER

#include <iostream>
#include <string>

#include "XmlDocument.hpp"
#include "XmlLog.hpp"


namespace Xml
{

    /*
     * Parse a XML from a stream
     *
     * @param xmlStream is the xml stream to parse
     * @param log is the output log
     */
    Document *
    parse(std::istream & xmlContent, Xml::Log * log = nullptr);

    /*
     * Parse a XML file
     *
     * @param path is the file path to parse
     * @param log is the output log
     */
    Document *
    load(std::string const & path, Xml::Log * log = nullptr);

    /*
     * Parse a XML from a string
     *
     * @param xmlContent is the xml string to parse
     * @param log is the output log
     */
    Document *
    parse(std::string const & xmlContent, Xml::Log * log = nullptr);

}

#endif //_H_XML_PARSER
