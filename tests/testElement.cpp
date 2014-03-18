#include <mk_test.h>

#include "../src/XmlElement.hpp"

using namespace Xml;

int
main()
{
    std::string name = "test";
    std::string tag  = "root";

    Element e(name);

    test_assert(e.name() == name);
    e.setName(tag);
    test_assert(e.name() == tag);

    auto c1 = new Element("child1");
    auto c2 = new Element("child2");

    e.append(c1);
    e.append(c2);

    test_assert(c1->parent() == &e);
    test_assert(c1->parentElement() == &e);
    test_assert(c2->parent() == &e);
    test_assert(c2->parentElement() == &e);

    test_assert(e.elements().size() == 2);
    test_assert(e.elements()[0] == c1);
    test_assert(e.elements()[1] == c2);

    std::string text = "This is a text in root.";

    e.appendText(text);
    test_assert(e.text() == text);

    std::string attr1 = "attr1";
    std::string attr2 = "attr2";
    std::string value1 = "value1";
    std::string value2 = "value2";

    c1->setAttribute(attr1, value1);
    c1->setAttribute(attr2, value2);

    test_assert(c1->attribute(attr1) == value1);
    test_assert(c1->attribute(attr2) == value2);

    std::string text1 = "This is a text in child 1.";
    std::string text2 = "This is a text in child 2.";
    c1->appendText(text1);
    c2->appendText(text2);

    test_assert(c1->text() == text1);
    test_assert(c2->text() == text2);

    c1->appendComment("This is a comment.");
    c1->appendComment("This is another comment.");

    auto c3 = new Element("child1.1");
    std::string text3 = "This is a text in child 1.1.";
    c3->appendText(text3);
    c3->appendComment("Yet another comment.");
    c1->append(c3);

    std::cerr << e << std::endl;

    e.setContent("Foo");
    test_assert(e.text() == "Foo");

    e.clearContent();
    test_assert(e.text() == "");
    test_assert(e.elements().size() == 0);

    return 0;
}