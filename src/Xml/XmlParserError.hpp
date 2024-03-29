#ifndef _H_XML_PARSER_ERROR
#define _H_XML_PARSER_ERROR

#include "XmlLog.hpp"


namespace Xml
{

    /**
     * Binds a XML log
     *
     * @param log is the log to bind
     */
    void
    parserBindLog(Xml::Log & log);

    /**
     * Output a lexical error in the bound log
     *
     * @param msg is the error message to log
     */
    void
    parserLexicalError(std::string const & msg);

    /**
     * Output a syntax error in the bound log
     *
     * @param msg is the error message to log
     */
    void
    parserSyntaxError(std::string const & msg);

    /**
     * Output a semantic error in the bound log
     *
     * @param msg is the error message to log
     */
    void
    parserSemanticError(std::string const & msg);

    /**
     * Check if parsing has definitely failed
     *
     * @return if has definitely failed by a syntax error
     */
    bool
    parserDefinitelyFailed();

}

#endif
