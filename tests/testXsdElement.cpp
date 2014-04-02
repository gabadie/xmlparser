
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

        elementInstruction.regex(regex, xsdDoc, xsdDoc->root());

        test_assert(regex == "<hello>");
    }

    delete xsdDoc;
}

int
main()
{
    testXsdElementRegex();

    return 0;
}
