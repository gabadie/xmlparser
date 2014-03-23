#include <mk_test.h>
#include <sstream>

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
    test_assert(doc->root() != 0);

    if (doc == 0 || doc->root() == 0)
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
    test_assert(doc->root() != 0);

    if (doc == 0 || doc->root() == 0)
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
        <!-- this is a comment before the root -->
        <hello>
            <!-- this is a comment -->
            <!-- this is a comment 2-->
            <!--this is a comment 3-->
            <!--this is a comment 4 -->
            <balise1></balise1>
            <!-- this is another comment -->
        </hello>
        <!-- this is a comment after the root -->
    ));
    Xml::Log log;

    Xml::Document * doc = Xml::parse(content, &log);

    test_assert(doc != 0);
    test_assert(doc->root() != 0);

    if (doc == 0 || doc->root() == 0)
    {
        return;
    }

    //std::cerr << content << std::endl;
    //std::cerr << *doc << std::endl;

    {
        std::stringstream ss;
        ss << *doc->children()[0];
        //test_assert(ss.str() == "<!-- this is a comment before the root -->");
    }

    {
        std::stringstream ss;
        ss << *doc->root()->children()[0];
        test_assert(ss.str() == "<!-- this is a comment -->");
    }

    {
        std::stringstream ss;
        ss << *doc->root()->children()[1];
        test_assert(ss.str() == "<!-- this is a comment 2-->");
    }

    {
        std::stringstream ss;
        ss << *doc->root()->children()[2];
        test_assert(ss.str() == "<!--this is a comment 3-->");
    }

    {
        std::stringstream ss;
        ss << *doc->root()->children()[3];
        test_assert(ss.str() == "<!--this is a comment 4 -->");
    }

    {
        std::stringstream ss;
        ss << *doc->root()->children()[5];
        test_assert(ss.str() == "<!-- this is another comment -->");
    }

    {
        std::stringstream ss;
        ss << *doc->children()[2];
        test_assert(ss.str() == "<!-- this is a comment after the root -->");
    }

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
