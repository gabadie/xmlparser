
#include <sstream>

#include "XmlParserError.hpp"

#include "../MemoryLeakTrackerOn.hpp"

namespace
{
    Xml::Log * logTarget = nullptr;
    bool syntaxErrors = false;
}

/*
 * Flex file number
 */
extern
int
yylineno;

/*
 * Flex file number
 */
extern
char
yytext[];

namespace Xml
{

    void
    parserBindLog(Xml::Log & log)
    {
        logTarget = &log;
        syntaxErrors = false;
    }

    void
    parserLexicalError(std::string const & msg)
    {
        std::ostringstream lineStream;

        lineStream << "line ";
        lineStream << yylineno;
        lineStream << " (lexical error): ";
        lineStream << msg;

        logTarget->append(lineStream.str());
    }

    void
    parserSyntaxError(std::string const & msg)
    {
        std::ostringstream lineStream;

        lineStream << "line ";
        lineStream << yylineno;
        lineStream << " (syntax error): ";
        lineStream << msg;

        logTarget->append(lineStream.str());
        syntaxErrors = true;
    }

    void
    parserSemanticError(std::string const & msg)
    {
        logTarget->append("semantic error: " + msg);
    }

    bool
    parserDefinitelyFailed()
    {
        return syntaxErrors;
    }

}

#include "../MemoryLeakTrackerOff.hpp"
