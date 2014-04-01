
#include <sstream>

#include "testhelper.hpp"

/*
 * To test the Xml parser, we need to access some protected/private field
 */
#define protected public
#define private public

#include "../src/Xml/XmlDoctype.hpp"
#include "../src/Xml/XmlParser.hpp"

void
testXmlDoctypeNode()
{
    std::string content = "HTML";
    std::string formatted = "<!DOCTYPE " + content + ">";
    Xml::Doctype xmlDoctype(content);

    test_assert(xmlDoctype.contentText() == "");
    test_assert(xmlDoctype.isElement() == false);

    {
        std::ostringstream oss;
        oss << xmlDoctype;
        test_assert(oss.str() == formatted);
    }
}

void
testXmlDoctypeParsing1()
{
    std::string content (xml_code(
        <!DOCTYPE HTML>
        <root>
        </root>
    ));

    Xml::Log log;
    Xml::Document * doc = Xml::parse(content, &log);

    std::cerr << log;

    test_assert(doc != nullptr);
    if(doc == nullptr) return;

    test_assert(doc->root() != nullptr);
    if(doc->root() == nullptr) return;

    test_assert(doc->children().size() > 1);

    {
        std::stringstream ss;
        ss << *doc->children()[0];
        test_assert(ss.str() == "<!DOCTYPE HTML>");
    }

    delete doc;
}

void
testXmlDoctypeParsing2()
{
        //<!DOCTYPE math SYSTEM "http://www.w3.org/Math/DTD/mathml1/mathml.dtd">
    std::string content (xml_code(
        <!DOCTYPE HTML>
        <root>
        </root>
    ));

    Xml::Log log;
    Xml::Document * doc = Xml::parse(content, &log);

    std::cerr << log;

    test_assert(doc != nullptr);
    if(doc == nullptr) return;

    test_assert(doc->root() != nullptr);
    if(doc->root() == nullptr) return;

    test_assert(doc->children().size() > 1);

    {
        std::stringstream ss;
        ss << *doc->children()[0];
        std::cerr << ss.str() << std::endl;
        test_assert(ss.str() == "<!DOCTYPE math SYSTEM "
            "\"http://www.w3.org/Math/DTD/mathml1/mathml.dtd\">");
    }

    delete doc;
}

int
main()
{
    testXmlDoctypeNode();
    std::cerr << "TEST n°1" << std::endl;
    testXmlDoctypeParsing1();
    //std::cerr << "TEST n°2" << std::endl;
    //testXmlDoctypeParsing1();

    return 0;
}
