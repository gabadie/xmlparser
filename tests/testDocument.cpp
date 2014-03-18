#include <mk_test.h>

#include <algorithm>

#include "../src/XmlComment.hpp"
#include "../src/XmlDocument.hpp"
#include "../src/XmlElement.hpp"

using namespace Xml;

int
main()
{
    Document doc;

    std::string name = "test";
    std::string tag  = "root";

    auto root = new Element(name);

    test_assert(root->name() == name);
    root->setName(tag);
    test_assert(root->name() == tag);


    doc.append(new Comment("This is a comment at the beginning of the XML document."));

    doc.setRoot(root);

    doc.append(new Comment("This is a comment at the end of the XML document."));

    test_assert(doc.root() == root);

    test_assert(doc.children().size() == 3);

    auto c1 = new Element("child1");
    auto c2 = new Element("child2");

    root->append(c1);
    root->append(c2);

    test_assert(c1->parent() == root);
    test_assert(c1->parentElement() == root);
    test_assert(c2->parent() == root);
    test_assert(c2->parentElement() == root);

    test_assert(root->elements().size() == 2);
    test_assert(root->elements()[0] == c1);
    test_assert(root->elements()[1] == c2);

    std::string text = "This is a text in root.";

    root->appendText(text);
    test_assert(root->text() == text);

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

    std::cerr << doc << std::endl;

    //doc.saveToFile("test.xml");

    auto const children = root->children();
    test_assert(std::find(std::begin(children), std::end(children), c1) != std::end(children));
    root->remove(c1);
    auto const children2 = root->children();
    test_assert(std::find(std::begin(children2), std::end(children2), c1) == std::end(children2));

    root->setContent("Foo");
    test_assert(root->text() == "Foo");

    root->clearContent();
    test_assert(root->text() == "");
    test_assert(root->elements().size() == 0);

    return 0;
}
