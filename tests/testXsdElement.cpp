
#include "testhelper.hpp"

#include "../src/Xsd/XsdElement.hpp"


void
testXsdElementRegex()
{
    std::string xsd = xml_code(
        <xsd:element name="hello" />
    );

    Xml::Log log;
    auto xsdDoc = Xml::parse(xsd, &log);

    {
        Xsd::Element elementInstruction;

        std::string regex;

        elementInstruction.regex(regex, xsdDoc, xsdDoc->root(), log);

        test_assert(regex == "<hello>");
    }

    delete xsdDoc;
}

bool
testhelperValidateElement(std::string const & xml, std::string const & xsd)
{
    bool r = false;

    Xml::Log log;
    auto xmlDoc = Xml::parse(xml, &log);
    auto xsdDoc = Xml::parse(xsd, &log);

    {
        Xsd::Element elementInstruction;

        r = elementInstruction.validate(xmlDoc->root(), xsdDoc, xsdDoc->root(), log);
    }

    delete xmlDoc;
    delete xsdDoc;

    return r;
}

void
testXsdValidate()
{
    std::string xml = xml_code(
        <foo:bar />
    );

    std::string xsd = xml_code(
        <xsd:element name="foo:bar" />
    );

    test_assert(testhelperValidateElement(xml, xsd) == true);
}

void
testXsdMissingName()
{
    std::string xml = xml_code(
        <foo:bar />
    );

    std::string xsd = xml_code(
        <xsd:element />
    );

    test_assert(testhelperValidateElement(xml, xsd) == false);
}

void
testXsdBadTag()
{
    std::string xml = xml_code(
        <foo:br />
    );

    std::string xsd = xml_code(
        <xsd:element name="foo:bar" />
    );

    test_assert(testhelperValidateElement(xml, xsd) == false);
}


int
main()
{
    testXsdElementRegex();
    testXsdValidate();
    testXsdMissingName();
    testXsdBadTag();

    return 0;
}
