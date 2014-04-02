#include <sstream>

#include "testhelper.hpp"

/*
 * To test the Xml parser, we need to access some protected/private field
 */
#define protected public
#define private public

#include "../src/Xsd2/Xsd.hpp"
#include "../src/Xml/XmlParser.hpp"
#include "../src/Xml/XmlDocument.hpp"

void
testXsdBasics()
{
    std::string content (xml_code(
        <root>
            <balise1></balise1>
            <balise2 />
        </root>
    ));

    Xml::Log log;

    Xml::Document * doc = Xml::parse(content, &log);

    test_assert(doc != nullptr);
    if (doc == nullptr) return;

    test_assert(doc->root() != nullptr);
    if (doc->root() == nullptr) return;

    test_assert(Xsd::stringifyElement(doc->root()) == "<balise1><balise2>");
}

int
main()
{
    testXsdBasics();

    return 0;
}
