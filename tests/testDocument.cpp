
#include <sstream>

#include "testhelper.hpp"

/*
 * To test the Xml parser, we need to access some protected/private field
 */
#define protected public
#define private public

#include "../src/Xml/XmlComment.hpp"
#include "../src/Xml/XmlDocument.hpp"
#include "../src/Xml/XmlElement.hpp"
#include "../src/Xml/XmlProcessingInstruction.hpp"
#include "../src/Xml/XmlText.hpp"

void
testXmlDocument()
{
    Xml::Document doc;

    // Add a processing instruction before the xml tree
    doc.appendProcessingInstruction("xml", "version", "1.0", "encoding", "UTF-8");

    test_assert(doc.children().size() == 1);

    {
        auto pi = dynamic_cast<Xml::ProcessingInstruction *>(doc.children()[0]);

        test_assert(pi != nullptr);
        test_assert(pi->attribute("version")  == "1.0");
        test_assert(pi->attribute("encoding") == "UTF-8");
    }

    // Add a comment before the xml tree
    std::string comment = "This is a comment at the beginning of the XML document.";
    doc.appendComment(comment);

    test_assert(doc.children().size() == 2);

    {
        auto xmlComment = dynamic_cast<Xml::Comment *>(doc.children()[1]);
        test_assert(xmlComment != nullptr);
        std::string formattedComment = "<!--" + comment + "-->";
        std::ostringstream oss;
        *xmlComment >> oss;
        test_assert(oss.str() == formattedComment);
    }

    // Add a root to the xml document
    {
        std::string name = "root";
        auto root = new Xml::Element(name);

        doc.setRoot(root);
        test_assert(root->parent() == &doc);
        test_assert(doc.children().size() == 3);

        {
            auto xmlRoot = dynamic_cast<Xml::Element *>(doc.children()[2]);
            test_assert(xmlRoot != nullptr);
            test_assert(xmlRoot == root);
        }

        {
            auto xmlRoot = doc.root();
            test_assert(xmlRoot != nullptr);
            test_assert(xmlRoot == root);
        }
    }

    // Replace the xml document's root
    {
        std::string name = "new_root";
        auto root = new Xml::Element(name);

        doc.setRoot(root);
        test_assert(root->parent() == &doc);
        test_assert(doc.children().size() == 3);

        {
            auto xmlRoot = dynamic_cast<Xml::Element *>(doc.children()[2]);
            test_assert(xmlRoot != nullptr);
            test_assert(xmlRoot == root);
        }

        {
            auto xmlRoot = doc.root();
            test_assert(xmlRoot != nullptr);
            test_assert(xmlRoot == root);
        }

        {
            auto xmlElt1 = new Xml::Element("child1");
            root->append(xmlElt1);

            test_assert(xmlElt1->parent() == root);
            test_assert(xmlElt1->document()->root() != nullptr);
            test_assert(xmlElt1->document()->root() == root);
        }
    }

    // Add a comment after the xml tree
    std::string endComment = "This is a comment at the end of the XML document.";
    doc.appendComment(endComment);

    test_assert(doc.children().size() == 4);

    {
        auto xmlComment = dynamic_cast<Xml::Comment *>(doc.children()[3]);
        test_assert(xmlComment != nullptr);
        std::string formattedComment = "<!--" + endComment + "-->";
        std::ostringstream oss;
        *xmlComment >> oss;
        test_assert(oss.str() == formattedComment);
    }
}

int
main()
{
    testXmlDocument();

    return 0;
}
