#include <mk_test.h>

#include "../src/XmlParser.hpp"

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
    for (Xml::Element const * e : doc->root()->elements("balise1"))
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

    std::cerr << doc->root()->text() << "\n";

    //test_assert(doc->root()->text() == "Hello World");

    delete doc;
}


int
main()
{
    test_elements_basic();
    test_elements_errors();
    test_text();

    return 0;
}
