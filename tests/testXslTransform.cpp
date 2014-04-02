
#include "testhelper.hpp"

#include "../src/Xml/Xml.hpp"
#include "../src/Xsl/Xsl.hpp"


void
testXslSeveralRoot()
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

int
main()
{
    testXslSeveralRoot();
    testXslNoRoot();
    testXslBadDocumentAppend();

    return 0;
}
