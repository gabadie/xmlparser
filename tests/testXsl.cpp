#include "testhelper.hpp"

/*
 * To test the Xml parser, we need to access some protected/private field
 */
#define protected public
#define private public

#include "../src/Xml/XmlParser.hpp"
#include "../src/Xsl/Xsl.hpp"
#include "../src/Xsl/XslTemplate.hpp"


void
testSimpleXsl()
{
    // xml
    std::string xmlContent (xml_code(
        <catalog>
                <root>
                    <cd>
                        <title>Title A</title>
                    </cd>

                    <cd>
                        <title>Title B</title>
                    </cd>
                </root>
        </catalog>
    ));
    Xml::Log xmlLog;
    Xml::Document * xmlDoc = Xml::parse(xmlContent, &xmlLog);

    // xsl
    std::string xslContent (xml_code(
        <xsl:stylesheet>
            <xsl:template match="cd">
                <a>lol</a>
                <xsl:value-of select="title" />
            </xsl:template>
        </xsl:stylesheet>
    ));
    Xml::Log xslLog;
    Xml::Document * xslDoc = Xml::parse(xslContent, &xslLog);

    test_assert(xslDoc != 0);
    test_assert(xmlDoc != 0);

    Xml::Log transformLog;
    Xml::Document* result = Xsl::transform(*xmlDoc, *xslDoc, transformLog);

    delete xmlDoc;
    delete xslDoc;
    delete result;
}


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

            <xsl:template match="/">
                <html>

                <xsl:for-each select="cd">
                    <div class="song">
                        <h1><xsl:value-of select="title" /></h1>
                        <h2>(this is a cd)</h2>
                    </div>
                </xsl:for-each>

                </html>
            </xsl:template>

        </xsl:stylesheet>
    ));
    Xml::Log xslLog;
    Xml::Document * xslDoc = Xml::parse(xslContent, &xslLog);

    test_assert(xslDoc != 0);
    test_assert(xmlDoc != 0);

    Xml::Log transformLog;
    Xml::Document* result = Xsl::transform(*xmlDoc, *xslDoc, transformLog);

    delete xmlDoc;
    delete xslDoc;
    delete result;
}

void
testGetTemplate()
{
    // xml
    std::string xmlContent (xml_code(
        <catalog>
            <cd>
                <title>Title A</title>
                <artist>Artist A</artist>
                <category>Category A</category>
            </cd>

            <cd>
                <title>Title B</title>
                <artist>Artist B</artist>
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

            <xsl:template match="category">
                FirstTemplate
            </xsl:template>

            <xsl:template match="category">
                LastTemplate
            </xsl:template>
        </xsl:stylesheet>
    ));
    Xml::Log xslLog;
    Xml::Document * xslDoc = Xml::parse(xslContent, &xslLog);

    test_assert(xmlDoc != 0);
    test_assert(xslDoc != 0);

    const Xml::Element* cdElement = xmlDoc->root()->elements()[0];
    const Xml::Element* titleElement = cdElement->elementsByTag("title")[0];
    const Xml::Element* artistElement = cdElement->elementsByTag("artist")[0];
    const Xml::Element* categoryElement = cdElement->elementsByTag("category")[0];

    const Xml::Element* titleTemplate = Xsl::getTemplate(*xslDoc, titleElement);
    test_assert(titleTemplate != nullptr);
    test_assert(titleTemplate->attribute("match") == "cd/title");

    const Xml::Element* cdTemplate = Xsl::getTemplate(*xslDoc, cdElement);
    test_assert(cdTemplate != nullptr);
    test_assert(cdTemplate->attribute("match") == "catalog/cd");

    const Xml::Element* categoryTemplate = Xsl::getTemplate(*xslDoc, categoryElement);
    test_assert(categoryTemplate != nullptr);
    test_assert(categoryTemplate->children()[0]->contentText() == "LastTemplate");

    const Xml::Element* artistTemplate = Xsl::getTemplate(*xslDoc, artistElement);
    test_assert(artistTemplate == nullptr);

    delete xmlDoc;
    delete xslDoc;
}

void
testValueOf()
{
    // xml
    std::string xmlContent (xml_code(
        <root>A<tag>B</tag>C</root>
    ));
    Xml::Log xmlLog;
    Xml::Document * xmlDoc = Xml::parse(xmlContent, &xmlLog);

    // xsl
    std::string xslContent (xml_code(
        <xsl:stylesheet>
            <xsl:template match="/">
                <result><xsl:value-of select="." /></result>
            </xsl:template>
        </xsl:stylesheet>
    ));
    Xml::Log xslLog;
    Xml::Document * xslDoc = Xml::parse(xslContent, &xslLog);

    test_assert(xslDoc != 0);
    test_assert(xmlDoc != 0);

    Xml::Log transformLog;
    Xml::Document* result = Xsl::transform(*xmlDoc, *xslDoc, transformLog);

    test_assert(result->root()->children()[0]->contentText() == "ABC");

    delete xmlDoc;
    delete xslDoc;
    delete result;
}

void
testApplyTemplates()
{
    // xml
    std::string xmlContent (xml_code(
    <catalog>
        <cd>
            <title>Empire Burlesque</title>
            <artist>Bob Dylan</artist>
            <country>USA</country>
            <company>Columbia</company>
            <price>10.90</price>
            <year>1985</year>
            <!-- dspodfdps -->
        </cd>
        <cd>
            <title>Hide your heart</title>
            <artist>Bonnie Tyler</artist>
            <country>UK</country>
            <company>CBS Records</company>
            <price>9.90</price>
            <year>1988</year>
        </cd>
    </catalog>


    ));

    Xml::Log xmlLog;
    Xml::Document * xmlDoc = Xml::parse(xmlContent, &xmlLog);

    // xsl
    std::string xslContent (xml_code(
        <xsl:stylesheet>
        <xsl:template match="/">
              <html>
              <body>
              <h2>My CD Collection</h2>
              <xsl:apply-templates />
              </body>
              </html>
            </xsl:template>

            <xsl:template match="cd">
              <p>
                <xsl:apply-templates select="title"/>
                <xsl:apply-templates select="artist"/>
              </p>
            </xsl:template>

            <xsl:template match="title">
              Title: <span style="color:#ff0000">
              <xsl:value-of select="."/></span>
              <br />
            </xsl:template>

            <xsl:template match="artist">
              Artist: <span style="color:#00ff00">
              <xsl:value-of select="."/></span>
              <br />
            </xsl:template>
        </xsl:stylesheet>


    ));

    Xml::Log xslLog;
    Xml::Document * xslDoc = Xml::parse(xslContent, &xslLog);

    test_assert(xslDoc != 0);
    test_assert(xmlDoc != 0);

    Xml::Log transformLog;
    Xml::Document* result = Xsl::transform(*xmlDoc, *xslDoc, transformLog);

    //std::cerr << transformLog << std::endl;
    // TODO : add asserts

    delete xmlDoc;
    delete xslDoc;
    delete result;
}

int main()
{

    testXslTransform();
    testGetTemplate();
    testValueOf();
    testSimpleXsl();
    testApplyTemplates();

    return 0;
}
