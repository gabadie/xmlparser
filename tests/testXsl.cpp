
#include "testhelper.hpp"

#include "../src/Xml/XmlParser.hpp"
#include "../src/Xsl/Xsl.hpp"

void
testXslTransform()
{

    // xml
    std::string xmlContent (xml_code(
        <root>
            rethdghg
            <cd>
                <title>Title A</title>
            </cd>

            <cd>
                <title>Title B</title>
            </cd>
        </root>
    ));

    Xml::Log xmlLog;

    Xml::Document * xmlDoc = Xml::parse(xmlContent, &xmlLog);


    // xsl
    std::string xslContent (xml_code(
        <xsl:stylesheet>    
            <xsl:template match="cd">
                <xsl:value-of select="title" />
            </xsl:template>
        </xsl:stylesheet>
    ));
    Xml::Log xslLog;
    Xml::Document * xslDoc = Xml::parse(xslContent, &xslLog);

    Xml::Document* result = Xsl::xslTransform(*xmlDoc, *xslDoc);

    std::cerr << *result << std::endl;

    delete xmlDoc;
    delete xslDoc;
}

void
testGetTemplate()
{

    // xml
    std::string xmlContent (xml_code(
        <catalog>
            <cd>
                <title>Title A</title>
                <category>Category A</category>
            </cd>

            <cd>
                <title>Title B</title>
                <category>Category B</category>
            </cd>
        </catalog>
    ));

    Xml::Log xmlLog;

    Xml::Document * xmlDoc = Xml::parse(xmlContent, &xmlLog);


    // xsl
    std::string xslContent (xml_code(
        <xsl:stylesheet>
            <xsl:template match="cd">
                DUMMY
            </xsl:template>

            <xsl:template match="catalog/cd">
                <xsl:value-of select="title" /> - <xsl:value-of select="category" />
            </xsl:template>

            <xsl:template match="cd/title">
                <xsl:value-of select="." />
            </xsl:template>
        </xsl:stylesheet>
    ));
    Xml::Log xslLog;
    Xml::Document * xslDoc = Xml::parse(xslContent, &xslLog);


    const Xml::Element* cdElement = xmlDoc->root()->elements()[0];
    const Xml::Element* titleElement = cdElement->elements("title")[0];
    const Xml::Element* categoryElement = cdElement->elements("category")[0];

    const Xml::Element* titleTemplate = Xsl::getTemplate(*xslDoc, titleElement);
    test_assert(titleTemplate != nullptr);
    test_assert(titleTemplate->attribute("match") == "cd/title");

    const Xml::Element* cdTemplate = Xsl::getTemplate(*xslDoc, cdElement);
    test_assert(cdTemplate != nullptr);
    test_assert(cdTemplate->attribute("match") == "catalog/cd");

    const Xml::Element* categoryTemplate = Xsl::getTemplate(*xslDoc, categoryElement);
    test_assert(categoryTemplate == nullptr);

    free(xmlDoc);
    free(xslDoc);
}

int
main()
{
   testXslTransform();
   testGetTemplate();

    return 0;
}
