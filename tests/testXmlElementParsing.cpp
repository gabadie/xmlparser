
#include "testhelper.hpp"

/*
 * To test the Xml parser, we need to access some protected/private field
 */
#define protected public
#define private public

#include "../src/Xml/XmlElement.hpp"
#include "../src/Xml/XmlParser.hpp"


void
testXmlElementParsingBasic()
{
    std::string content (xml_code(
        <hello>
            <balise1>
            </balise1>
            <balise2>
            </balise2>
        </hello>
    ));
    Xml::Log log;

    Xml::Document * doc = Xml::parse(content, &log);

    test_assert(doc != 0);
    test_assert(doc->root() != 0);

    if (doc == 0 || doc->root() == 0)
    {
        return;
    }

    test_assert(doc->root()->name() == "hello");
    test_assert(doc->root()->name() != "hllo");

    test_assert(doc->root()->elements()[0]->name() == "balise1");
    test_assert(doc->root()->elements()[1]->name() == "balise2");
    test_assert(doc->root()->elements().size() == 2);

    delete doc;
}

void
testXmlElementParsingUnclosed()
{
    std::string content (xml_code(
        <hello>
            <balise1>
            <balise1>
            <balise2>
            </balise2>
        </hello>
    ));
    Xml::Log log;

    Xml::Document * doc = Xml::parse(content, &log);

    test_assert(doc == 0);
}

void
testXmlElementParsingBadClose()
{
    std::string content (xml_code(
        <hello>
            <balise1>
            </balise3>
            <balise2>
            </balise2>
        </hello>
    ));
    Xml::Log log;

    Xml::Document * doc = Xml::parse(content, &log);

    test_assert(doc != 0);
    test_assert(doc->root()->elements("balise1").size() == 0);
    test_assert(doc->root()->elements("balise2").size() == 1);

    delete doc;
}


int
main()
{
    testXmlElementParsingBasic();
    testXmlElementParsingUnclosed();
    testXmlElementParsingBadClose();

    return 0;
}
