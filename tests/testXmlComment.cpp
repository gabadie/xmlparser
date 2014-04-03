
#include <sstream>

#include "testhelper.hpp"

/*
 * To test the Xml parser, we need to access some protected/private field
 */
#define protected public
#define private public

#include "../src/Xml/XmlComment.hpp"
#include "../src/Xml/XmlParser.hpp"

void
testXmlCommentNode()
{
    std::string comment = "This is a comment.";
    std::string formattedComment = "<!--" + comment + "-->";
    Xml::Comment xmlComment(comment);

    test_assert(xmlComment.contentText() == "");
    test_assert(xmlComment.isElement() == false);

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
testXmlCommentParsing()
{
    std::string content (xml_code(
        <!-- this is a comment before the root -->
        <hello>
            <!-- this is a comment -->
            <!-- this is a comment 2-->
            <!--this is a comment 3-->
            <!--this is a comment 4 -->
            <balise1></balise1>
            <!-- this is another comment -->
        </hello>
        <!-- this is a comment after the root -->
    ));
    Xml::Log log;

    Xml::Document * doc = Xml::parse(content, &log);

    test_assert(doc != 0);
    test_assert(doc->root() != 0);

    if (doc == 0 || doc->root() == 0)
    {
        return;
    }

    {
        std::stringstream ss;
        ss << *doc->children()[0];
        test_assert(ss.str() == "<!-- this is a comment before the root -->");
    }

    {
        std::stringstream ss;
        ss << *doc->root()->children()[0];
        test_assert(ss.str() == "<!-- this is a comment -->");
    }

    {
        std::stringstream ss;
        ss << *doc->root()->children()[1];
        test_assert(ss.str() == "<!-- this is a comment 2-->");
    }

    {
        std::stringstream ss;
        ss << *doc->root()->children()[2];
        test_assert(ss.str() == "<!--this is a comment 3-->");
    }

    {
        std::stringstream ss;
        ss << *doc->root()->children()[3];
        test_assert(ss.str() == "<!--this is a comment 4 -->");
    }

    {
        std::stringstream ss;
        ss << *doc->root()->children()[5];
        test_assert(ss.str() == "<!-- this is another comment -->");
    }

    {
        std::stringstream ss;
        ss << *doc->children()[2];
        test_assert(ss.str() == "<!-- this is a comment after the root -->");
    }

    delete doc;
}

int
main()
{
    testXmlCommentNode();
    testXmlCommentParsing();

    return 0;
}
