
#include "testhelper.hpp"

/*
 * To test the Xml parser, we need to access some protected/private field
 */
#define protected public
#define private public

/*
 * We test Xml::Node with Xml::Element
 */
#include "../src/Xml/XmlDocument.hpp"
#include "../src/Xml/XmlElement.hpp"


void
testXmlNodeParent()
{
    auto * root = new Xml::Element("root");

    { // test parents
        test_assert(root->parent() == nullptr);
        test_assert(root->document() == nullptr);
    }

    auto c1 = new Xml::Element("child1");
    auto c2 = new Xml::Element("child2");

    root->append(c1);
    root->append(c2);

    { // test parents & documents
        test_assert(root->parent() == nullptr);
        test_assert(root->document() == nullptr);
        test_assert(c1->parent() == root);
        test_assert(c1->document() == nullptr);
        test_assert(c2->parent() == root);
        test_assert(c2->document() == nullptr);
    }

    auto c3 = new Xml::Element("child3");

    c2->append(c3);

    { // test parents & documents
        test_assert(c3->parent() == c2);
        test_assert(c3->document() == nullptr);
    }

    Xml::Document doc (root);

    { // test parents & documents
        test_assert(root->parent() == &doc);
        test_assert(root->document() == &doc);
        test_assert(c1->parent() == root);
        test_assert(c1->document() == &doc);
        test_assert(c2->parent() == root);
        test_assert(c2->document() == &doc);
        test_assert(c3->parent() == c2);
        test_assert(c3->document() == &doc);
    }
}

void
testXmlNodeDeletion()
{
    Xml::Element root("root");

    auto c1 = new Xml::Element("child1");
    auto c2 = new Xml::Element("child2");

    root.append(c1);
    root.append(c2);

    {
        test_assert(root.elements().size() == 2);
        test_assert(root.elements()[0]->name() == "child1");
        test_assert(root.elements()[1]->name() == "child2");
    }

    root.remove(c1);

    {
        test_assert(root.elements().size() == 1);
        test_assert(root.elements()[0]->name() == "child2");
        test_assert(c1->parent() == nullptr);
    }

    delete c1;
    delete c2;

    {
        test_assert(root.elements().size() == 0);
    }
}


int
main()
{
    testXmlNodeParent();
    testXmlNodeDeletion();

    return 0;
}
