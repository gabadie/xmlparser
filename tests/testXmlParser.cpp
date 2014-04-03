
#include <mk_test.h>

#include "../src/Xml/XmlParser.hpp"


void
testXmlNonExistingFile()
{
    Xml::Log log;
    Xml::Document * doc = Xml::load("./unexisting.xml", &log);

    test_assert(doc == nullptr);
}

void
testXmlSyntaxError()
{
    Xml::Log log;
    Xml::Document * doc = Xml::load("./xml_original_files/syntax_error.xml", &log);

    test_assert(doc == nullptr);
    test_assert(log.find("line 2") == 1);
}

void
testXmlLexicalError()
{
    Xml::Log log;
    Xml::Document * doc = Xml::load("./xml_original_files/lexical_error.xml", &log);

    test_assert(doc == nullptr);
    test_assert(log.find("line 1 (lexical error)") == 1);
}


int
main()
{
    testXmlNonExistingFile();
    testXmlSyntaxError();
    testXmlLexicalError();

    return 0;
}
