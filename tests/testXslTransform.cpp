
#include "testhelper.hpp"

#include "../src/Xml/Xml.hpp"
#include "../src/Xsl/Xsl.hpp"


void
testXslSeveralsRoot()
{
    std::string xml = xml_code(
        <root />
    );

    std::string xsl = xml_code(
        <xsl:stylesheet>
            <xsl:template match="root">
                <root1 />
                <root2 />
            </xsl:template>
        </xsl:stylesheet>
    );

    Xml::Log log;
    auto xmlDoc = Xml::parse(xml);
    auto xslDoc = Xml::parse(xsl);

    test_assert(xmlDoc != nullptr);
    test_assert(xslDoc != nullptr);

    auto xmlNewDoc = Xsl::transform(*xmlDoc, *xslDoc, log);

    test_assert(xmlNewDoc->root()->tag() == "root1");

    delete xmlDoc;
    delete xslDoc;
    delete xmlNewDoc;
}

void
testXslNoRoot()
{
    std::string xml = xml_code(
        <root />
    );

    std::string xsl = xml_code(
        <xsl:stylesheet>
            <xsl:template match="root">
                <!--hello world-->
            </xsl:template>
        </xsl:stylesheet>
    );

    Xml::Log log;
    auto xmlDoc = Xml::parse(xml);
    auto xslDoc = Xml::parse(xsl);

    test_assert(xmlDoc != nullptr);
    test_assert(xslDoc != nullptr);

    auto xmlNewDoc = Xsl::transform(*xmlDoc, *xslDoc, log);

    test_assert(xmlNewDoc->root() == nullptr);

    delete xmlDoc;
    delete xslDoc;
    delete xmlNewDoc;
}

void
testXslBadDocumentAppend()
{
    std::string xml = xml_code(
        <root />
    );

    std::string xsl = xml_code(
        <xsl:stylesheet>
            <xsl:template match="root">
                <!--hello world-->
                Foo
                <root1 />
                Bar
            </xsl:template>
        </xsl:stylesheet>
    );

    Xml::Log log;
    auto xmlDoc = Xml::parse(xml);
    auto xslDoc = Xml::parse(xsl);

    test_assert(xmlDoc != nullptr);
    test_assert(xslDoc != nullptr);

    auto xmlNewDoc = Xsl::transform(*xmlDoc, *xslDoc, log);

    test_assert(xmlNewDoc->root() != nullptr);
    test_assert(xmlNewDoc->root()->tag() == "root1");

    delete xmlDoc;
    delete xslDoc;
    delete xmlNewDoc;
}


void
testIgnoredXMLTags()
{
    std::string xml = xml_code(
        <root>
            Some text
            <![CDATA[ Some cdata stuff ]]>
            <?PITarget PIContent?>
            <!-- a comment -->
        </root>
    );

    std::string xsl = xml_code(
        <xsl:stylesheet>
            <xsl:template match="/">
                <root>
                    <xsl:apply-templates />
                </root>
            </xsl:template>
        </xsl:stylesheet>
    );

    Xml::Log log;
    auto xmlDoc = Xml::parse(xml);
    auto xslDoc = Xml::parse(xsl);

    test_assert(xmlDoc != nullptr);
    test_assert(xslDoc != nullptr);

    auto xmlNewDoc = Xsl::transform(*xmlDoc, *xslDoc, log);

    // std::cerr << xmlNewDoc->root()->children().size() << std::endl;
    test_assert(xmlNewDoc->root() != nullptr);
    //test_assert(xmlNewDoc->root()->children().size() == 2);
    //test_assert(xmlNewDoc->root()->children()[0]->objectLabel() == Xml::ObjectLabel::Text);
    //test_assert(xmlNewDoc->root()->children()[1]->objectLabel() == Xml::ObjectLabel::CharacterData);

    delete xmlDoc;
    delete xslDoc;
    delete xmlNewDoc;
}


int
main()
{
    testXslSeveralsRoot();
    testXslNoRoot();
    testXslBadDocumentAppend();
    testIgnoredXMLTags();

    return 0;
}
