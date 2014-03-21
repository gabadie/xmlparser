
#include "XmlParserError.hpp"


namespace
{
    Xml::Log * logTarget = nullptr;
    bool syntaxErrors = false;
}

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
        logTarget->append("lexical error: " + msg);
    }

    void
    parserSyntaxError(std::string const & msg)
    {
        logTarget->append("syntax error: " + msg);
        syntaxErrors = true;
    }

    void
    parserSemanticError(std::string const & msg)
    {
        logTarget->append("semantic error: " + msg);
    }

    bool
    parserDefenetlyFailed()
    {
        return syntaxErrors;
    }

}
