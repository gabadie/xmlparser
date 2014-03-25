
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

void
testXmlElementParsingAttributes()
{
    std::string content (xml_code(
        <hello>
            <balise1 attr1="value1">
            </balise1>
            <balise2 attr2="value2" attr3="value3">
            </balise2>
        </hello>
    ));
    Xml::Log log;

    Xml::Document * doc = Xml::parse(content, &log);

    test_assert(doc != nullptr);

    auto balise1 = doc->root()->elements("balise1");
    auto balise2 = doc->root()->elements("balise2");

    test_assert(balise1.size() == 1);
    test_assert(balise2.size() == 1);
    test_assert(balise1[0]->attribute("attr1") == "value1");
    test_assert(balise2[0]->attribute("attr2") == "value2");
    test_assert(balise2[0]->attribute("attr3") == "value3");

    delete doc;
}

void
testXmlElementParsingBadAttributes()
{
    std::string content (xml_code(
        <hello>
            <balise1 "value1">
            </balise1>
            <balise2 attr2= attr3="value3">
            </balise2>
        </hello>
    ));
    Xml::Log log;

    Xml::Document * doc = Xml::parse(content, &log);

    std::cerr << log << std::endl;

    test_assert(doc != nullptr);

    auto balise1 = doc->root()->elements("balise1");
    auto balise2 = doc->root()->elements("balise2");

    test_assert(balise1.size() == 1);
    test_assert(balise2.size() == 1);
    test_assert(balise1[0]->attribute("attr1") == "");
    test_assert(balise2[0]->attribute("attr2") == "");
    test_assert(balise2[0]->attribute("attr3") == "value3");

    delete doc;
}

int
main()
{
    testXmlElementParsingBasic();
    testXmlElementParsingUnclosed();
    testXmlElementParsingBadClose();
    testXmlElementParsingAttributes();
    //testXmlElementParsingBadAttributes();

    return 0;
}
