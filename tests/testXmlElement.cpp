
#include "testhelper.hpp"

/*
 * To test the Xml parser, we need to access some protected/private field
 */
#define protected public
#define private public

#include "../src/Xml/XmlConsts.hpp"
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
    test_assert(root.tag() == name);

    std::string tag  = "root";

    root.setName(tag);

    test_assert(root.name() == tag);
    test_assert(root.tag() == tag);
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
testXmlElementNamespaceAttributes()
{
    Xml::Element root("root");

    std::string attr1 = "hello:attr1";
    std::string value1 = "value1";
    std::string attr2 = "attr2";
    std::string value2 = "value2";

    {
        auto attrs = root.namespaceAttributes("hello");
        test_assert(attrs.find("attr1") == std::end(attrs));
    }

    {
        auto attrs = root.namespaceAttributes("");
        test_assert(attrs.find("attr1") == std::end(attrs));
    }

    {
        auto attrs = root.namespaceAttributes("");
        test_assert(attrs.find(attr2) == std::end(attrs));
    }

    root.setAttribute(attr1, value1);

    {
        auto attrs = root.namespaceAttributes("hello");
        test_assert(attrs.find("attr1") != std::end(attrs));
    }

    {
        auto attrs = root.namespaceAttributes("world");
        test_assert(attrs.find("attr1") == std::end(attrs));
    }

    {
        auto attrs = root.namespaceAttributes("");
        test_assert(attrs.find(attr2) == std::end(attrs));
    }

    root.setAttribute(attr2, value2);

    {
        auto attrs = root.namespaceAttributes("");
        test_assert(attrs.find(attr2) != std::end(attrs));
    }

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

void
testXmlElementClone()
{
    std::string originalName = "origin";
    Xml::Element* origin = new Xml::Element(originalName);
    Xml::Element* clone = dynamic_cast<Xml::Element *>(origin->clone());

    test_assert(clone != nullptr);
    test_assert(origin->name() == clone->name());
    test_assert(clone->children().size() == 0);

    std::string content = "This is the new content of the element";
    std::string newName = "newname";
    origin->setContent(content);
    origin->setName(newName);

    test_assert(origin->children().size() == 1);
    test_assert(clone->children().size() == 0);
    test_assert(origin->name() == newName && clone->name() == originalName);

    delete origin;
    delete clone;
}

void
testXmlElementNamespace()
{
    Xml::Element root("root");

    auto xmlElt1_1 = new Xml::Element("elt1", "ns");
    root.append(xmlElt1_1);

    test_assert(root.elements().size() == 1);
    test_assert(root.elements()[0] == xmlElt1_1);
    test_assert(root.elements("ns:elt1").size() == 1);
    test_assert(root.elements("ns:elt1")[0] == xmlElt1_1);

    auto xmlElt1_2 = new Xml::Element("elt1", "ns");
    root.append(xmlElt1_2);

    test_assert(root.elements().size() == 2);
    test_assert(root.elements()[0] == xmlElt1_1);
    test_assert(root.elements()[1] == xmlElt1_2);
    test_assert(root.elements("ns:elt1").size() == 2);
    test_assert(root.elements("ns:elt1")[0] == xmlElt1_1);
    test_assert(root.elements("ns:elt1")[1] == xmlElt1_2);

    auto xmlElt2 = new Xml::Element("elt2", "namespace");
    root.append(xmlElt2);

    test_assert(root.elements().size() == 3);
    test_assert(root.elements()[0] == xmlElt1_1);
    test_assert(root.elements()[1] == xmlElt1_2);
    test_assert(root.elements()[2] == xmlElt2);
    test_assert(root.elements("namespace:elt2").size() == 1);
    test_assert(root.elements("namespace:elt2")[0] == xmlElt2);
}

void testXmlElementMatches() {
    Xml::Document xmlDoc;

    auto root = new Xml::Element("root");
    auto child = new Xml::Element("child");
    auto subchild = new Xml::Element("subchild");

    xmlDoc.setRoot(root);
    root->append(child);
    child->append(subchild);

    test_assert(root->matches("/"));
    test_assert(child->matches("child"));
    test_assert(subchild->matches("subchild"));
    test_assert(subchild->matches("child/subchild"));
    test_assert(subchild->matches("root/child/subchild"));

    test_assert(!child->matches("/"));
    test_assert(!child->matches("subchild"));
    test_assert(!subchild->matches("child/root/subchild"));
}

void
testXmlElementFullText()
{
    Xml::Element root("root");

    root.appendText("hello");

    test_assert(root.contentText() == "");
    test_assert(root.text() == "hello");
    test_assert(root.fullText() == "hello");

    Xml::Element * xmlElt1 = new Xml::Element("elt1");
    std::string const text1 = "Text in elt1";
    xmlElt1->appendText(text1);
    root.append(xmlElt1);

    test_assert(root.text() == "hello");
    test_assert(root.fullText() == std::string("hello") + Xml::CAT_SEPARATOR + text1);

    Xml::Element * xmlElt11 = new Xml::Element("elt11");
    std::string const text11 = "Text in elt11";
    xmlElt11->appendText(text11);
    root.append(xmlElt11);

    test_assert(root.text() == "hello");
    test_assert(root.fullText() == std::string("hello") + Xml::CAT_SEPARATOR + text1 + Xml::CAT_SEPARATOR + text11);

    root.appendText("hello");
    test_assert(root.fullText() == std::string("hello") + Xml::CAT_SEPARATOR + text1 + Xml::CAT_SEPARATOR + text11 + "hello");
}

void
testXmlElementValueOf()
{
    Xml::Element root("root");

    root.appendText("A");

    Xml::Element * e = new Xml::Element("tag");
    e->appendText("B");
    root.append(e);

    root.appendText("C");

    test_assert(root.valueOf(".") == "ABC");
}


int
main()
{
    testXmlElementBasics();
    testXmlElementTagname();
    testXmlElementAttributes();
    testXmlElementNamespaceAttributes();
    testXmlElementChildren();
    testXmlElementContent();
    testXmlElementClone();
    testXmlElementNamespace();
    testXmlElementMatches();
    testXmlElementFullText();
    testXmlElementValueOf();

    return 0;
}
