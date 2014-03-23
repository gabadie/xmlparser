#include <mk_test.h>

#include <algorithm>
#include <sstream>
#include <utility>

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

            static
            void
            XmlDocument();

            static
            void
            XPath();
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

            Element root(name);

            test_assert(root.name() == name);

            std::string tag  = "root";
            root.setName(tag);
            test_assert(root.name() == tag);

            std::string attr1 = "attr1";
            std::string value1 = "value1";
            root.setAttribute(attr1, value1);
            test_assert(root.attribute(attr1) == value1);

            std::string attr2 = "attr2";
            std::string value2 = "value2";
            root.setAttribute(attr1, value1);
            test_assert(root.attribute(attr1) == value1);
            root.setAttribute(attr2, value2);
            test_assert(root.attribute(attr2) == value2);
        }

        // Add Element children
        {
            Element root("root");

            auto c1 = new Element("child1");
            auto c2 = new Element("child2");

            root.append(c1);
            root.append(c2);

            test_assert(root.hasChild(c1));
            test_assert(root.hasChild(c2));

            auto c3 = new Element("child3");
            c2->append(c3);
            test_assert(c2->hasChild(c3));
            test_assert(root.hasChild(c3));

            test_assert(c1->parent() == &root);
            test_assert(c1->parentElement() == &root);
            test_assert(c2->parent() == &root);
            test_assert(c2->parentElement() == &root);

            test_assert(root.elements().size() == 2);
            test_assert(root.elements()[0] == c1);
            test_assert(root.elements()[1] == c2);

            test_assert(root.children().size() == 2);
            test_assert(root.children()[0] == c1);
            test_assert(root.children()[1] == c2);
        }

        // Add various children + deletion
        {
            Element root("root");

            auto xmlElt1 = new Element("elt1");
            root.append(xmlElt1);

            std::string text = "This is a text.";
            root.appendText(text);
            Text * xmlText = dynamic_cast<Text *>(root.children()[1]);
            test_assert(xmlText != nullptr);

            auto xmlElt2 = new Element("elt2");
            root.append(xmlElt2);

            std::string comment = "This is a comment.";
            root.appendComment(comment);
            Comment * xmlComment = dynamic_cast<Comment *>(root.children()[3]);
            test_assert(xmlComment != nullptr);

            root.appendProcessingInstruction("xml", "version", "1.0", "encoding", "UTF-8");
            ProcessingInstruction * xmlPI = dynamic_cast<ProcessingInstruction *>(root.children()[4]);
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

        // Set content
        {
            Element root("root");

            auto xmlElt1 = new Element("elt1");
            root.append(xmlElt1);

            test_assert(root.elements().size() == 1);
            test_assert(root.elements()[0] == xmlElt1);

            std::string content = "This is the new content of the element";
            root.setContent(content);

            test_assert(root.elements().size() == 0);
            test_assert(root.children().size() == 1);
            test_assert(dynamic_cast<Text *>(root.children()[0]) != nullptr);
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

    void
    Test::XmlDocument()
    {
        Document doc;

        // Add a processing instruction before the xml tree
        doc.appendProcessingInstruction("xml", "version", "1.0", "encoding", "UTF-8");

        test_assert(doc.children().size() == 1);

        {
            auto pi = dynamic_cast<ProcessingInstruction *>(doc.children()[0]);

            test_assert(pi != nullptr);
            test_assert(pi->attribute("version")  == "1.0");
            test_assert(pi->attribute("encoding") == "UTF-8");
        }

        // Add a comment before the xml tree
        std::string comment = "This is a comment at the beginning of the XML document.";
        doc.appendComment(comment);

        test_assert(doc.children().size() == 2);

        {
            auto xmlComment = dynamic_cast<Comment *>(doc.children()[1]);
            test_assert(xmlComment != nullptr);
            std::string formattedComment = "<!--" + comment + "-->";
            std::ostringstream oss;
            *xmlComment >> oss;
            test_assert(oss.str() == formattedComment);
        }

        // Add a root to the xml document
        {
            std::string name = "root";
            auto root = new Element(name);

            doc.setRoot(root);
            test_assert(root->parent() == &doc);
            test_assert(doc.children().size() == 3);

            {
                auto xmlRoot = dynamic_cast<Element *>(doc.children()[2]);
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
            auto root = new Element(name);

            doc.setRoot(root);
            test_assert(root->parent() == &doc);
            test_assert(doc.children().size() == 3);

            {
                auto xmlRoot = dynamic_cast<Element *>(doc.children()[2]);
                test_assert(xmlRoot != nullptr);
                test_assert(xmlRoot == root);
            }

            {
                auto xmlRoot = doc.root();
                test_assert(xmlRoot != nullptr);
                test_assert(xmlRoot == root);
            }

            {
                auto xmlElt1 = new Element("child1");
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
            auto xmlComment = dynamic_cast<Comment *>(doc.children()[3]);
            test_assert(xmlComment != nullptr);
            std::string formattedComment = "<!--" + endComment + "-->";
            std::ostringstream oss;
            *xmlComment >> oss;
            test_assert(oss.str() == formattedComment);
        }
    }


    /*
     * <root>
     *   <elt1>
     *     <elt11></elt11>
     *     <elt11></elt11>
     *  </elt1>
     *  <elt2>
     *     <elt21></elt21>
     *     <elt22></elt22>
     *  </elt2>
     * </root>
     */
    void
    Test::XPath()
    {
        Document doc;

        std::string root = "root";
        auto xmlRoot = new Element(root);
        doc.setRoot(xmlRoot);

        std::string elt1 = "elt1";
        auto xmlElt1 = new Element(elt1);
        xmlRoot->append(xmlElt1);

        std::string elt11 = "elt11";
        auto xmlElt11_1 = new Element(elt11);
        xmlElt1->append(xmlElt11_1);
        auto xmlElt11_2 = new Element(elt11);
        xmlElt1->append(xmlElt11_2);

        std::string elt2 = "elt2";
        auto xmlElt2 = new Element(elt2);
        xmlRoot->append(xmlElt2);

        std::string elt21 = "elt21";
        auto xmlElt21 = new Element(elt21);
        xmlElt2->append(xmlElt21);

        std::string elt22 = "elt22";
        auto xmlElt22 = new Element(elt22);
        xmlElt2->append(xmlElt22);

        // Lambda to get the nth element of the container returned by Element::select
        static
        auto const
        get = [](decltype(std::declval<Element &>().select("")) const & r, std::size_t n)
        {
            auto it = std::begin(r);
            std::advance(it, n);
            return *it;
        };

        // Query: "/"
        {
            static
            auto const
            rootQueryTest = [&](Element const * elt)
            {
                auto results = elt->select("/");
                test_assert(results.size() == 1);
                test_assert(get(results, 0) == xmlRoot);
            };

            rootQueryTest(xmlRoot);
            rootQueryTest(xmlElt1);
            rootQueryTest(xmlElt11_1);
            rootQueryTest(xmlElt11_2);
            rootQueryTest(xmlElt2);
            rootQueryTest(xmlElt21);
            rootQueryTest(xmlElt22);
        }

        // Query: "."
        {

            static
            auto const
            selfQueryTest = [&](Element const * elt)
            {
                auto results = elt->select(".");
                test_assert(results.size() == 1);
                test_assert(get(results, 0) == elt);
            };

            selfQueryTest(xmlRoot);
            selfQueryTest(xmlElt1);
            selfQueryTest(xmlElt11_1);
            selfQueryTest(xmlElt11_2);
            selfQueryTest(xmlElt2);
            selfQueryTest(xmlElt21);
            selfQueryTest(xmlElt22);
        }

        // Query: ".."
        {

            static
            auto const
            parentQueryTest = [&](Element const * elt, Element const * result)
            {
                auto results = elt->select("..");
                if(result != nullptr)
                {
                    test_assert(results.size() == 1);
                    test_assert(get(results, 0) == result);
                }
                else
                {
                    test_assert(results.size() == 0);
                }
            };

            parentQueryTest(xmlRoot, nullptr);
            parentQueryTest(xmlElt1, xmlRoot);
            parentQueryTest(xmlElt11_1, xmlElt1);
            parentQueryTest(xmlElt11_2, xmlElt1);
            parentQueryTest(xmlElt2, xmlRoot);
            parentQueryTest(xmlElt21, xmlElt2);
            parentQueryTest(xmlElt22, xmlElt2);
        }

        // Query: "tag"
        {
            {
                auto results = xmlRoot->select(elt1);
                test_assert(results.size() == 1);
                test_assert(get(results, 0) == xmlElt1);
            }

            {
                auto results = xmlRoot->select(elt2);
                test_assert(results.size() == 1);
                test_assert(get(results, 0) == xmlElt2);
            }

            {
                auto results = xmlElt1->select(elt11);
                test_assert(results.size() == 2);
                test_assert(get(results, 0) == xmlElt11_1);
                test_assert(get(results, 1) == xmlElt11_2);
            }

            {
                auto results = xmlElt2->select(elt21);
                test_assert(results.size() == 1);
                test_assert(get(results, 0) == xmlElt21);
            }

            {
                auto results = xmlElt2->select(elt22);
                test_assert(results.size() == 1);
                test_assert(get(results, 0) == xmlElt22);
            }

            {
                auto results = xmlRoot->select("12456789");
                test_assert(results.size() == 0);
            }
        }

        // Query: "/tag"
        {
            static
            auto const
            rootQueryTest = [&](Element const * elt, Element const * rootChild)
            {
                auto results = elt->select("/" + rootChild->name());
                test_assert(results.size() == 1);
                test_assert(get(results, 0) == rootChild);
            };

            rootQueryTest(xmlRoot, xmlElt1);
            rootQueryTest(xmlElt1, xmlElt1);
            rootQueryTest(xmlElt11_1, xmlElt1);
            rootQueryTest(xmlElt11_2, xmlElt1);
            rootQueryTest(xmlElt2, xmlElt1);
            rootQueryTest(xmlElt21, xmlElt1);
            rootQueryTest(xmlElt22, xmlElt1);

            rootQueryTest(xmlRoot, xmlElt2);
            rootQueryTest(xmlElt1, xmlElt2);
            rootQueryTest(xmlElt11_1, xmlElt2);
            rootQueryTest(xmlElt11_2, xmlElt2);
            rootQueryTest(xmlElt2, xmlElt2);
            rootQueryTest(xmlElt21, xmlElt2);
            rootQueryTest(xmlElt22, xmlElt2);

            {
                auto results = xmlElt1->select("/" + root);
                test_assert(results.size() == 0);
            }
        }

        // Query: "tag1/tag2"
        {
            {
                auto results = xmlRoot->select(elt1 + "/" + elt11);
                test_assert(results.size() == 2);
                test_assert(get(results, 0) == xmlElt11_1);
                test_assert(get(results, 1) == xmlElt11_2);
            }

            {
                auto results = xmlRoot->select(elt2 + "/" + elt21);
                test_assert(results.size() == 1);
                test_assert(get(results, 0) == xmlElt21);
            }

            {
                auto results = xmlRoot->select(elt2 + "/" + elt22);
                test_assert(results.size() == 1);
                test_assert(get(results, 0) == xmlElt22);
            }

            {
                auto results = xmlRoot->select(elt1 + "/" + "12456789");
                test_assert(results.size() == 0);
            }
        }

        // Query: "/tag1/tag2"
        {
            {
                auto results = xmlRoot->select("/" + elt1 + "/" + elt11);
                test_assert(results.size() == 2);
                test_assert(get(results, 0) == xmlElt11_1);
                test_assert(get(results, 1) == xmlElt11_2);
            }

            {
                auto results = xmlRoot->select("/" + elt2 + "/" + elt21);
                test_assert(results.size() == 1);
                test_assert(get(results, 0) == xmlElt21);
            }

            {
                auto results = xmlRoot->select("/" + elt2 + "/" + elt22);
                test_assert(results.size() == 1);
                test_assert(get(results, 0) == xmlElt22);
            }

            {
                auto results = xmlRoot->select("/" + elt1 + "/" + "12456789");
                test_assert(results.size() == 0);
            }
        }

        std::string elt111 = "elt111";
        auto xmlElt111_1 = new Element(elt111);
        xmlElt11_1->append(xmlElt111_1);
        auto xmlElt111_2 = new Element(elt111);
        xmlElt11_1->append(xmlElt111_2);

        std::string elt1111 = "elt1111";
        auto xmlElt111_21 = new Element(elt1111);
        xmlElt111_2->append(xmlElt111_21);

        // Query: "tag1/.../tagN"
        {
            {
                auto results = xmlRoot->select(elt1 + "/" + elt11 + "/" + elt111);
                test_assert(results.size() == 2);
                test_assert(get(results, 0) == xmlElt111_1);
                test_assert(get(results, 1) == xmlElt111_2);
            }

            {
                auto results = xmlRoot->select(elt1 + "/" + elt11 + "/" + elt111 + "/" + elt1111);
                test_assert(results.size() == 1);
                test_assert(get(results, 0) == xmlElt111_21);
            }
        }

        // Query: "/tag1/.../tagN"
        {
            {
                auto results = xmlRoot->select("/" + elt1 + "/" + elt11 + "/" + elt111);
                test_assert(results.size() == 2);
                test_assert(get(results, 0) == xmlElt111_1);
                test_assert(get(results, 1) == xmlElt111_2);
            }

            {
                auto results = xmlRoot->select("/" + elt1 + "/" + elt11 + "/" + elt111 + "/" + elt1111);
                test_assert(results.size() == 1);
                test_assert(get(results, 0) == xmlElt111_21);
            }
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
    Test::XmlDocument();
    Test::XPath();

    return 0;
}
