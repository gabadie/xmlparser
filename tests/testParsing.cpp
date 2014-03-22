#include <mk_test.h>

#include "../src/Xml/XmlParser.hpp"

#define xml_code(code) \
    ((const char *) #code)

void
test_elements_basic()
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

    if (doc == 0)
    {
        return;
    }

    test_assert(doc->root()->name() == "hello");
    test_assert(doc->root()->name() != "hllo");

    int i = 0;
    for (Xml::Element const * e : doc->root()->elements())
    {
        test_assert(e->name() == "balise1" || i != 0);
        test_assert(e->name() == "balise2" || i != 1);
        test_assert(i < 2);

        i++;
    }

    delete doc;
}

void
test_elements_errors()
{
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

    {
        std::string content (xml_code(
            <hello>
                <balise1>
                <balise2>
                </balise2>
            </hello>
        ));
        Xml::Log log;

        Xml::Document * doc = Xml::parse(content, &log);

        test_assert(doc == 0);
    }

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
    }
}

void
test_text()
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

    if (doc == 0)
    {
        return;
    }

    test_assert(doc->root()->text() == "Hello\nWorld");

    delete doc;
}

void
test_comment()
{
    std::string content (xml_code(
        <hello>
            Hello
            <balise1></balise2>
            <!-- this is another comment -->
        </hello>
    ));
    Xml::Log log;

    Xml::Document * doc = Xml::parse(content, &log);

    test_assert(doc != 0);

    if (doc == 0)
    {
        return;
    }

    // test_assert(doc->root()->text() == "Hello\nWorld");

    delete doc;
}

void
test_syntax_error()
{
    Xml::Log log;
    Xml::Document * doc = Xml::load("./xml_original_files/syntax_error.xml", &log);

    test_assert(doc == 0);
    test_assert(log.find("line 2") == 1);

    delete doc;
}

void
test_lexical_error()
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
    test_elements_basic();
    test_elements_errors();
    test_text();
    test_comment();
    test_syntax_error();
    test_lexical_error();

    return 0;
}
