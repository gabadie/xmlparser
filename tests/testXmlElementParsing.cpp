
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
testXmlElementParsingNamespace()
{
    std::string content (xml_code(
        <xs:element>
            <xs:complexType>
                <xs:sequence>
                    <xs:element/>
                </xs:sequence>
            </xs:complexType>
        </xs:element>
    ));
    Xml::Log log;

    Xml::Document * doc = Xml::parse(content, &log);

    test_assert(doc != 0);
    test_assert(doc->root() != 0);

    if (doc == 0 || doc->root() == 0)
    {
        return;
    }

    test_assert(doc->root()->name() == "element");
    test_assert(doc->root()->namespaceName() == "xs");
    test_assert(doc->root()->tag() == "xs:element");

    test_assert(doc->root()->elements()[0]->name() == "complexType");
    test_assert(doc->root()->elements()[0]->namespaceName() == "xs");
    test_assert(doc->root()->elements()[0]->elements()[0]->tag() == "xs:sequence");
    test_assert(doc->root()->elements()[0]->elements()[0]->elements()[0]->tag() == "xs:element");

    delete doc;
}

void
testXmlElementParsingUnclosed()
{
    std::string content (R"XML(
        <hello>
            <balise1>
            <balise1>
            <balise2>
            </balise2>
        </hello>
    )XML");
    Xml::Log log;

    Xml::Document * doc = Xml::parse(content, &log);

    test_assert(doc != 0);
    test_assert(doc->root() != 0);

    test_assert(doc->root()->tag() == "hello");
    test_assert(doc->root()->mChildren.size() == 1);
    test_assert(doc->root()->elements()[0]->tag() == "balise1");
    test_assert(doc->root()->elements()[0]->mChildren.size() == 1);
    test_assert(doc->root()->elements()[0]->elements()[0]->tag() == "balise1");
    test_assert(doc->root()->elements()[0]->elements()[0]->mChildren.size() == 1);
    test_assert(doc->root()->elements()[0]->elements()[0]->elements()[0]->tag() == "balise2");

    test_assert(log.find("unexpected </hello>") == 1);
    test_assert(log.find("missing closing element </balise1>") == 3);
    test_assert(log.find("missing closing element </hello>") == 5);

    delete doc;
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
    test_assert(doc->root()->elements("balise1").size() == 1);
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
testXmlElementParsingAttributesNamespace()
{
    std::string content (xml_code(
        <hello>
            <balise1 xs:attr1="value1">
            </balise1>
            <balise2 xsd:attr2="value2" nspc:attr3="value3">
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
    test_assert(balise1[0]->attribute("xs:attr1") == "value1");
    test_assert(balise2[0]->attribute("xsd:attr2") == "value2");
    test_assert(balise2[0]->attribute("nspc:attr3") == "value3");

    delete doc;
}

void
testXmlElementParsingBadAttributes()
{
    std::string content (xml_code(
        <hello>
            <balise1 ="value1" attr4="value4" "value5" "value7" attr6="value6">
            </balise1>
            <balise2 attr2= attr3="value3">
            </balise2>
        </hello>
    ));
    Xml::Log log;

    Xml::Document * doc = Xml::parse(content, &log);

    test_assert(doc != nullptr);

    auto balise1 = doc->root()->elements("balise1");
    auto balise2 = doc->root()->elements("balise2");

    // If the following tests don't pass, see in XmlParser.y the rule "atts"
    // The error may be caused by a shift/reduce conflict.
    test_assert(balise1.size() == 1);
    test_assert(balise2.size() == 1);
    test_assert(balise1[0]->attribute("attr4") == "value4");
    test_assert(balise1[0]->attribute("attr6") == "value6");
    test_assert(balise2[0]->attribute("attr2") == "");
    test_assert(balise2[0]->attribute("attr3") == "value3");

    delete doc;
}

void
testXmlElementParsingWoContent()
{
    std::string content (xml_code(
        <hello>
            <balise1 attr1="value1"/>
            <balise2 attr2="value2" attr3="value3"/>
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
testXmlElementParsingWoContentBad()
{
    std::string content (xml_code(
        <hello>
            <balise1 ="value1" attr4="value4" "value5" "value7" attr6="value6"/>
            <balise2 attr2= attr3="value3"/>
        </hello>
    ));
    Xml::Log log;

    Xml::Document * doc = Xml::parse(content, &log);

    test_assert(doc != nullptr);

    auto balise1 = doc->root()->elements("balise1");
    auto balise2 = doc->root()->elements("balise2");

    test_assert(balise1.size() == 1);
    test_assert(balise2.size() == 1);
    test_assert(balise1[0]->attribute("attr4") == "value4");
    test_assert(balise1[0]->attribute("attr6") == "value6");
    test_assert(balise2[0]->attribute("attr2") == "");
    test_assert(balise2[0]->attribute("attr3") == "value3");

    delete doc;
}

int
main()
{
    testXmlElementParsingBasic();
    testXmlElementParsingNamespace();
    testXmlElementParsingUnclosed();
    testXmlElementParsingBadClose();
    testXmlElementParsingAttributes();
    testXmlElementParsingAttributesNamespace();
    testXmlElementParsingBadAttributes();
    testXmlElementParsingWoContent();
    testXmlElementParsingWoContentBad();

    return 0;
}
