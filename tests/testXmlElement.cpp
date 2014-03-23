
#include "testhelper.hpp"

/*
 * To test the Xml parser, we need to access some protected/private field
 */
#define protected public
#define private public

#include "../src/Xml/XmlElement.hpp"
#include "../src/Xml/XmlComment.hpp"
#include "../src/Xml/XmlParser.hpp"
#include "../src/Xml/XmlProcessingInstruction.hpp"
#include "../src/Xml/XmlText.hpp"


void
testXmlElementBasics()
{
    Xml::Element root("root");

    test_assert(root.isElement() == true);

    root.appendText("hello");

    test_assert(root.contentText() == "");
    test_assert(root.text() == "hello");
}

void
testXmlElementTagname()
{
    std::string name = "test";

    Xml::Element root(name);

    test_assert(root.name() == name);

    std::string tag  = "root";

    root.setName(tag);

    test_assert(root.name() == tag);
}

void
testXmlElementAttributes()
{
    Xml::Element root("root");

    std::string attr1 = "attr1";
    std::string value1 = "value1";
    std::string attr2 = "attr2";
    std::string value2 = "value2";

    test_assert(attr1 != attr2);
    test_assert(value1 != value2);

    test_assert(root.attribute(attr1) == "");
    root.setAttribute(attr1, value1);
    test_assert(root.attribute(attr1) == value1);

    test_assert(root.attribute(attr2) == "");
    root.setAttribute(attr2, value2);
    test_assert(root.attribute(attr2) == value2);
}

void
testXmlElementChildren()
{
    Xml::Element root("root");

    { // empty children
        test_assert(root.elements().size() == 0);
        test_assert(root.mChildren.size() == 0);
    }

    auto c1 = new Xml::Element("child1");
    auto c2 = new Xml::Element("child2");

    root.append(c1);
    root.append(c2);

    { // check children
        test_assert(root.elements().size() == 2);
        test_assert(root.mChildren.size() == 2);

        test_assert(root.hasChild(c1));
        test_assert(root.hasChild(c2));
    }

    { // recursive children checking
        auto c3 = new Xml::Element("child3");

        c2->append(c3);

        test_assert(c2->hasChild(c3));
        test_assert(root.hasChild(c3));

        test_assert(c3->parent() == c2);
    }

    { // check element orders
        test_assert(root.elements()[0] == c1);
        test_assert(root.elements()[1] == c2);

        test_assert(root.children()[0] == c1);
        test_assert(root.children()[1] == c2);
    }
}

void
testXmlElementContent()
{
    Xml::Element root("root");

    auto xmlElt1 = new Xml::Element("elt1");
    root.append(xmlElt1);

    test_assert(root.elements().size() == 1);
    test_assert(root.elements()[0] == xmlElt1);

    std::string content = "This is the new content of the element";
    root.setContent(content);

    test_assert(root.elements().size() == 0);
    test_assert(root.mChildren.size() == 1);
    test_assert(dynamic_cast<Xml::Text *>(root.children()[0]) != nullptr);
}


int
main()
{
    testXmlElementBasics();
    testXmlElementTagname();
    testXmlElementAttributes();
    testXmlElementChildren();
    testXmlElementContent();

    return 0;
}
