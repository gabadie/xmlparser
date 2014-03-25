
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

#if 0

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

#endif

void
testXmlDocumentRoot()
{
    {
        auto root = new Xml::Element("root");
        Xml::Document doc(root);

        {
            test_assert(root->parent() == &doc);
            test_assert(doc.children().size() == 1);
            test_assert(doc.root() == root);
        }

        auto newRoot = new Xml::Element("root");

        doc.appendNode(newRoot);

        {
            test_assert(newRoot->parent() == &doc);
            test_assert(doc.children().size() == 1);
            test_assert(doc.root() == newRoot);
        }
    }

    {
        Xml::Document doc;
        auto root = new Xml::Element("root");

        {
            test_assert(doc.root() == nullptr);
        }

        doc.setRoot(root);

        {
            test_assert(root->parent() == &doc);
            test_assert(doc.children().size() == 1);
            test_assert(doc.root() == root);
        }

        doc.setRoot(nullptr);

        {
            test_assert(doc.children().size() == 0);
            test_assert(doc.root() == nullptr);
        }
    }
}

int
main()
{
    //testXmlDocument();
    testXmlDocumentRoot();

    return 0;
}
