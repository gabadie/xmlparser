#include <mk_test.h>

#include <algorithm>
#include <sstream>

#include "../src/Xml/XmlComment.hpp"
#include "../src/Xml/XmlDocument.hpp"
#include "../src/Xml/XmlElement.hpp"
#include "../src/Xml/XmlProcessingInstruction.hpp"
#include "../src/Xml/XmlText.hpp"

namespace Xml
{
    class Test
    {
        public:

            static
            void
            CommentNode();

            static
            void
            ElementNode();

            static
            void
            PINode();

            static
            void
            TextNode();
    };

    void
    Test::CommentNode()
    {
        std::string comment = "This is a comment.";
        std::string formattedComment = "<!--" + comment + "-->";
        Comment xmlComment(comment);

        test_assert(xmlComment.contentText() == "");

        {
            std::ostringstream oss;
            xmlComment >> oss;
            test_assert(oss.str() == formattedComment);
        }

        {
            std::ostringstream oss;
            oss << xmlComment;
            test_assert(oss.str() == formattedComment);
        }

        {
            std::ostringstream oss;
            xmlComment.exportToStream(oss, 0, "");
            test_assert(oss.str() == formattedComment);
        }
    }

    void
    Test::ElementNode()
    {
        // Basic tests
        {
            std::string name = "test";

            auto root = new Element(name);
            test_assert(root->name() == name);

            std::string tag  = "root";
            root->setName(tag);
            test_assert(root->name() == tag);

            std::string attr1 = "attr1";
            std::string value1 = "value1";
            root->setAttribute(attr1, value1);
            test_assert(root->attribute(attr1) == value1);

            std::string attr2 = "attr2";
            std::string value2 = "value2";
            root->setAttribute(attr1, value1);
            test_assert(root->attribute(attr1) == value1);
            root->setAttribute(attr2, value2);
            test_assert(root->attribute(attr2) == value2);
        }

        // Add Element children
        {
            auto root = new Element("root");

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

            test_assert(root->children().size() == 2);
            test_assert(root->children()[0] == c1);
            test_assert(root->children()[1] == c2);
        }

        // Add various children + deletion
        {
            auto root = new Element("root");

            auto xmlElt1 = new Element("elt1");
            root->append(xmlElt1);

            std::string text = "This is a text.";
            root->appendText(text);
            Text * xmlText = dynamic_cast<Text *>(root->children()[1]);
            test_assert(xmlText != nullptr);

            auto xmlElt2 = new Element("elt2");
            root->append(xmlElt2);

            std::string comment = "This is a comment.";
            root->appendComment(comment);
            Comment * xmlComment = dynamic_cast<Comment *>(root->children()[3]);
            test_assert(xmlComment != nullptr);

            root->appendProcessingInstruction("xml", "version", "1.0", "encoding", "UTF-8");
            ProcessingInstruction * xmlPI = dynamic_cast<ProcessingInstruction *>(root->children()[4]);
            test_assert(xmlPI != nullptr);

            test_assert(xmlElt1->parent() == root);
            test_assert(xmlElt1->parentElement() == root);
            test_assert(xmlText->parent() == root);
            test_assert(xmlElt2->parent() == root);
            test_assert(xmlElt2->parentElement() == root);
            test_assert(xmlComment->parent() == root);
            test_assert(xmlPI->parent() == root);

            test_assert(root->elements().size() == 2);
            test_assert(root->elements()[0] == xmlElt1);
            test_assert(root->elements()[1] == xmlElt2);

            test_assert(root->children().size() == 5);
            test_assert(root->children()[0] == xmlElt1);
            test_assert(root->children()[1] == xmlText);
            test_assert(root->children()[2] == xmlElt2);
            test_assert(root->children()[3] == xmlComment);
            test_assert(root->children()[4] == xmlPI);

            // Remove node
            {
                root->remove(xmlText);
                test_assert(root->elements().size() == 2);
                test_assert(root->children().size() == 4);
                test_assert(root->children()[0] == xmlElt1);
                test_assert(root->children()[1] == xmlElt2);
                test_assert(root->children()[2] == xmlComment);
                test_assert(root->children()[3] == xmlPI);
            }

            // Clear content
            {
                root->clearContent();
                test_assert(root->children().size() == 0);
            }
        }

        // Set content
        {
            auto root = new Element("root");

            auto xmlElt1 = new Element("elt1");
            root->append(xmlElt1);

            test_assert(root->elements().size() == 1);
            test_assert(root->elements()[0] == xmlElt1);

            std::string content = "This is the new content of the element";
            root->setContent(content);

            test_assert(root->elements().size() == 0);
            test_assert(root->children().size() == 1);
            test_assert(dynamic_cast<Text *>(root->children()[0]) != nullptr);
        }
    }

    void
    Test::PINode()
    {
        ProcessingInstruction xmlPI("xml", "version", "1.0", "encoding", "UTF-8");

        test_assert(xmlPI.mName == "xml");
        test_assert(xmlPI.attribute("version") == "1.0");
        test_assert(xmlPI.attribute("encoding") == "UTF-8");

        std::string pi = "<?xml encoding=\"UTF-8\" version=\"1.0\"?>";

        {
            std::ostringstream oss;
            xmlPI >> oss;
            test_assert(oss.str() == pi);
        }

        {
            std::ostringstream oss;
            oss << xmlPI;
            test_assert(oss.str() == pi);
        }

        {
            std::ostringstream oss;
            xmlPI.exportToStream(oss, 0, "");
            test_assert(oss.str() == pi);
        }

    }

    void
    Test::TextNode()
    {
        std::string text = "This is a text.";
        Text xmlText(text);

        test_assert(xmlText.contentText() == text);

        std::string text2 = "This is another text.";

        xmlText.setText(text2);

        test_assert(xmlText.contentText() == text2);

        {
            std::ostringstream oss;
            xmlText >> oss;
            test_assert(oss.str() == text2);
        }

        {
            std::ostringstream oss;
            oss << xmlText;
            test_assert(oss.str() == text2);
        }

        {
            std::ostringstream oss;
            xmlText.exportToStream(oss, 0, "");
            test_assert(oss.str() == text2);
        }
    }
}


using namespace Xml;

int
main()
{
    Test::CommentNode();
    Test::PINode();
    Test::TextNode();
    Test::ElementNode();

    return 0;

    Document doc;

    std::string name = "test";
    std::string tag  = "root";

    auto root = new Element(name);

    test_assert(root->name() == name);
    root->setName(tag);
    test_assert(root->name() == tag);


    doc.appendProcessingInstruction("xml", "version", "1.0", "encoding", "UTF-8");

    test_assert(static_cast<ProcessingInstruction *>(doc.children()[0])->attribute("version")  == "1.0");
    test_assert(static_cast<ProcessingInstruction *>(doc.children()[0])->attribute("encoding") == "UTF-8");

    doc.appendComment("This is a comment at the beginning of the XML document.");

    doc.setRoot(root);

    doc.appendComment("This is a comment at the end of the XML document.");

    test_assert(doc.root() == root);

    test_assert(doc.children().size() == 4);

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
