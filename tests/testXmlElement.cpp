
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
    std::string name = "test";

    Xml::Element root(name);

    test_assert(root.name() == name);

    { // change tag name
        std::string tag  = "root";

        root.setName(tag);

        test_assert(root.name() == tag);
    }

    { // attribute
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

    { // recusive children checking
        auto c3 = new Xml::Element("child3");

        c2->append(c3);

        test_assert(c2->hasChild(c3));
        test_assert(root.hasChild(c3));
    }

    { // test parents
        test_assert(c1->parent() == &root);
        test_assert(c1->parentElement() == &root);
        test_assert(c2->parent() == &root);
        test_assert(c2->parentElement() == &root);
    }

    { // check element orders
        test_assert(root.elements()[0] == c1);
        test_assert(root.elements()[1] == c2);

        test_assert(root.children()[0] == c1);
        test_assert(root.children()[1] == c2);
    }
}

void
testXmlElementDeletion()
{
    Xml::Element root("root");

    auto xmlElt1 = new Xml::Element("elt1");
    root.append(xmlElt1);

    std::string text = "This is a text.";
    root.appendText(text);
    Xml::Text * xmlText = dynamic_cast<Xml::Text *>(root.children()[1]);
    test_assert(xmlText != nullptr);

    auto xmlElt2 = new Xml::Element("elt2");
    root.append(xmlElt2);

    std::string comment = "This is a comment.";
    root.appendComment(comment);
    Xml::Comment * xmlComment = dynamic_cast<Xml::Comment *>(root.children()[3]);
    test_assert(xmlComment != nullptr);

    root.appendProcessingInstruction("xml", "version", "1.0", "encoding", "UTF-8");
    Xml::ProcessingInstruction * xmlPI = dynamic_cast<Xml::ProcessingInstruction *>(root.children()[4]);
    test_assert(xmlPI != nullptr);

    test_assert(xmlElt1->isElement());
    test_assert(xmlElt2->isElement());

    test_assert(xmlElt1->parent() == &root);
    test_assert(xmlElt1->parentElement() == &root);
    test_assert(xmlText->parent() == &root);
    test_assert(xmlElt2->parent() == &root);
    test_assert(xmlElt2->parentElement() == &root);
    test_assert(xmlComment->parent() == &root);
    test_assert(xmlPI->parent() == &root);

    test_assert(root.elements().size() == 2);
    test_assert(root.elements()[0] == xmlElt1);
    test_assert(root.elements()[1] == xmlElt2);

    test_assert(root.children().size() == 5);
    test_assert(root.children()[0] == xmlElt1);
    test_assert(root.children()[1] == xmlText);
    test_assert(root.children()[2] == xmlElt2);
    test_assert(root.children()[3] == xmlComment);
    test_assert(root.children()[4] == xmlPI);

    // Remove node
    {
        root.remove(xmlText);
        test_assert(root.elements().size() == 2);
        test_assert(root.children().size() == 4);
        test_assert(root.children()[0] == xmlElt1);
        test_assert(root.children()[1] == xmlElt2);
        test_assert(root.children()[2] == xmlComment);
        test_assert(root.children()[3] == xmlPI);
    }

    // Clear content
    {
        root.clearContent();
        test_assert(root.children().size() == 0);
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
    test_assert(root.children().size() == 1);
    test_assert(dynamic_cast<Xml::Text *>(root.children()[0]) != nullptr);
}


int
main()
{
    testXmlElementBasics();
    testXmlElementChildren();
    testXmlElementContent();

    return 0;
}
