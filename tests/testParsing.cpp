
#include <mk_test.h>

#include "../src/Xml/XmlParser.hpp"


void
testXmlSyntaxError()
{
    Xml::Log log;
    Xml::Document * doc = Xml::load("./xml_original_files/syntax_error.xml", &log);

    test_assert(doc == 0);
    test_assert(log.find("line 2") == 1);

    delete doc;
}

void
testXmlLexicalError()
{
    Xml::Log log;
    Xml::Document * doc = Xml::load("./xml_original_files/lexical_error.xml", &log);

    test_assert(doc == 0);
    test_assert(log.find("line 1 (lexical error)") == 1);

    delete doc;
}


int
main()
{
    testXmlSyntaxError();
    testXmlLexicalError();

    return 0;
}
