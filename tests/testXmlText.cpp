
#include <sstream>

#include "testhelper.hpp"

/*
 * To test the Xml parser, we need to access some protected/private field
 */
#define protected public
#define private public

#include "../src/Xml/XmlText.hpp"
#include "../src/Xml/XmlParser.hpp"

void
testXmlTextExport()
{
    std::string text = "This is a text.";
    Xml::Text xmlText(text);

    test_assert(xmlText.contentText() == text);

    {
        std::ostringstream oss;
        xmlText >> oss;
        test_assert(oss.str() == text);
    }

    {
        std::ostringstream oss;
        oss << xmlText;
        test_assert(oss.str() == text);
    }

    {
        std::ostringstream oss;
        xmlText.exportToStream(oss, 0, "");
        test_assert(oss.str() == text);
    }
}

void
testXmlTextParsing()
{
    {
        std::string content (xml_code(
            <hello>
                Hello
                <balise1>
                </balise1>
                World
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

        test_assert(doc->root()->mChildren[0]->contentText() == "Hello");
        test_assert(doc->root()->mChildren[2]->contentText() == "World");
        test_assert(doc->root()->text() == "Hello\nWorld");

        delete doc;
    }

    {
        std::string content (xml_code(
            Hello
            <hello>
                World
            </hello>
        ));

        Xml::Log log;
        Xml::Document * doc = Xml::parse(content, &log);

        test_assert(doc != nullptr);

        test_assert(doc->root() != nullptr);
        test_assert(doc->root()->name() == "hello");
        test_assert(doc->root()->text() == "World");

        delete doc;
    }

    {
        std::string content (xml_code(
            <hello>
                Hello
            </hello>
            World
        ));

        Xml::Log log;
        Xml::Document * doc = Xml::parse(content, &log);

        test_assert(doc != nullptr);

        test_assert(doc->root() != nullptr);
        test_assert(doc->root()->name() == "hello");
        test_assert(doc->root()->text() == "Hello");

        delete doc;
    }
}


int
main()
{
    testXmlTextExport();
    testXmlTextParsing();

    return 0;
}
