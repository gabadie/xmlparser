#include <mk_test.h>

#include "../src/XmlParser.hpp"

#define xml_code(code) \
    ((const char *) #code)

void
test_nodes()
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


int
main()
{
    test_nodes();

    return 0;
}
