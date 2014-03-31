
#include <sstream>

#include "testhelper.hpp"

/*
 * To test the Xml parser, we need to access some protected/private field
 */
#define protected public
#define private public

#include "../src/Xml/XmlCharacterData.hpp"
#include "../src/Xml/XmlParser.hpp"

void
testXmlCharacterDataNode()
{
    std::string content = "This is a <xml> comment.";
    std::string formatted = "<![CDATA[" + content + "]]>";
    Xml::CharacterData xmlCharacterData(content);

    test_assert(xmlCharacterData.contentText() == "");
    test_assert(xmlCharacterData.isElement() == false);

    {
        std::ostringstream oss;
        oss << xmlCharacterData;
        test_assert(oss.str() == formatted);
    }
}

void
testXmlCharacterDataParsing()
{
    // TODO: <root> <![CDATA[hello <world>]]> fails to parse
    std::string content (xml_code(
        <root><![CDATA[hello <world>]]>
        </root>
    ));

    Xml::Log log;
    Xml::Document * doc = Xml::parse(content, &log);

    test_assert(doc != 0);
    test_assert(doc->root() != 0);

    if (doc == 0 || doc->root() == 0)
    {
        return;
    }

    std::cerr << log;

    {
        std::stringstream ss;
        ss << *doc->root()->children()[0];
        test_assert(ss.str() == "<![CDATA[hello <world>]]>");
    }

    delete doc;
}

int
main()
{
    testXmlCharacterDataNode();
    testXmlCharacterDataParsing();

    return 0;
}
