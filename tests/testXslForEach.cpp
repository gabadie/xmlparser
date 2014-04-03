

/*
 * To test the Xml ValueOf, we need to access some protected/private field
 */
#define protected public
#define private public

#include "testhelper.hpp"

#include "../src/Xml/Xml.hpp"
#include "../src/Xsl/XslForEach.hpp"


void
testXslNormal()
{
    std::string xml = xml_code(
        <catalog>
            <cd>
                <title>Empire Burlesque</title>
                <artist>Bob Dylan</artist>
                <country>USA</country>
                <company>Columbia</company>
                <price>10.90</price>
                <year>1985</year>
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
    );

    std::string xsl = xml_code(
        <xsl:for-each select="cd" >
            each
        </xsl:for-each>
    );

    Xml::Log log;
    auto xmlDoc = Xml::parse(xml);
    auto xslDoc = Xml::parse(xsl);

    test_assert(xmlDoc != nullptr);
    test_assert(xslDoc != nullptr);

    Xsl::ForEach foreach;

    Xml::Log transformLog;
    auto r = foreach(xmlDoc->root(), *xslDoc, xslDoc->root(), transformLog);
    //std::cerr << ((Xml::Element *)r[0])->contentText() << std::endl;

    test_assert(r.size() == 2);
    test_assert(r[0]->contentText() == "each");
    test_assert(r[1]->contentText() == "each");

    delete xmlDoc;
    delete xslDoc;
}

void
testXslEmptySelect()
{
    std::string xml = xml_code(
        <catalog>
            <cd>
                <title>Empire Burlesque</title>
                <artist>Bob Dylan</artist>
                <country>USA</country>
                <company>Columbia</company>
                <price>10.90</price>
                <year>1985</year>
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
    );

    std::string xsl = xml_code(
        <xsl:for-each select="" >
        each
        </xsl:for-each>
    );

    Xml::Log log;
    auto xmlDoc = Xml::parse(xml);
    auto xslDoc = Xml::parse(xsl);

    test_assert(xmlDoc != nullptr);
    test_assert(xslDoc != nullptr);

    Xsl::ForEach foreach;

    Xml::Log transformLog;
    auto r = foreach(xmlDoc->root(), *xslDoc, xslDoc->root(), transformLog);
    //std::cerr << ((Xml::Element *)r[0])->contentText() << std::endl;

    test_assert(r.size() == 0);

    delete xmlDoc;
    delete xslDoc;
}

void
testXslOther()
{
    std::string xml = xml_code(
        <catalog>
            <cd>
                <title>Empire Burlesque</title>
                <artist>Bob Dylan</artist>
                <country>USA</country>
                <company>Columbia</company>
                <price>10.90</price>
                <year>1985</year>
            </cd>
            <cd>
                <title>Hide your heart</title>
                <artist>Bonnie Tyler</artist>
                <country>UK</country>
                <company>CBS Records</company>
                <price>9.90</price>
                <year>1988</year>
            </cd>
            <boot>
                <harm>
                </harm>
            </boot>
        </catalog>
    );

    std::string xsl = xml_code(
        <xsl:for-each select="/boot/harm" >
        each
        </xsl:for-each>
    );

    Xml::Log log;
    auto xmlDoc = Xml::parse(xml);
    auto xslDoc = Xml::parse(xsl);

    test_assert(xmlDoc != nullptr);
    test_assert(xslDoc != nullptr);

    Xsl::ForEach foreach;

    Xml::Log transformLog;
    auto r = foreach(xmlDoc->root(), *xslDoc, xslDoc->root(), transformLog);
    //std::cerr << ((Xml::Element *)r[0])->contentText() << std::endl;

    test_assert(r.size() == 1);
    test_assert(r[0]->contentText() == "each");

    delete xmlDoc;
    delete xslDoc;
}


void
testXslParam()
{
    std::string xml = xml_code(
        <catalog>
            <cd param="myParam">
                <title>Empire Burlesque</title>
                <artist>Bob Dylan</artist>
                <country>USA</country>
                <company>Columbia</company>
                <price>10.90</price>
                <year>1985</year>
            </cd>
            <cd param="myParam">
                <title>Hide your heart</title>
                <artist>Bonnie Tyler</artist>
                <country>UK</country>
                <company>CBS Records</company>
                <price>9.90</price>
                <year>1988</year>
            </cd>
            <cd param="myParam">
                <title>Empire state</title>
                <artist>Bob robert</artist>
                <country>turkey</country>
                <company>Columbia</company>
                <price>10.90</price>
                <year>1985</year>
            </cd>
            <boot>
                <harm>
                </harm>
            </boot>
        </catalog>
    );

    std::string xsl = xml_code(
        <xsl:for-each select="/cd/@param" >
        each
        </xsl:for-each>
    );

    Xml::Log log;
    auto xmlDoc = Xml::parse(xml);
    auto xslDoc = Xml::parse(xsl);

    test_assert(xmlDoc != nullptr);
    test_assert(xslDoc != nullptr);

    Xsl::ForEach foreach;

    Xml::Log transformLog;
    auto r = foreach(xmlDoc->root(), *xslDoc, xslDoc->root(), transformLog);
    std::cerr << ((Xml::Element *)r[0])->contentText() << std::endl;

    //test_assert(r.size() == 1);
    //test_assert(r[0]->contentText() == "each");

    delete xmlDoc;
    delete xslDoc;
}


void
testXslNoTemplate()
{
    std::string xml = xml_code(
        <catalog>
            <cd>
                <title>Empire Burlesque</title>
                <artist>Bob Dylan</artist>
                <country>USA</country>
                <company>Columbia</company>
                <price>10.90</price>
                <year>1985</year>
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
    );

    std::string xsl = xml_code(
        <xsl:for-each select="cd" >
        </xsl:for-each>
    );

    Xml::Log log;
    auto xmlDoc = Xml::parse(xml);
    auto xslDoc = Xml::parse(xsl);

    test_assert(xmlDoc != nullptr);
    test_assert(xslDoc != nullptr);

    Xsl::ForEach foreach;

    Xml::Log transformLog;
    auto r = foreach(xmlDoc->root(), *xslDoc, xslDoc->root(), transformLog);
    //std::cerr << ((Xml::Element *)r[0])->contentText() << std::endl;

    test_assert(r.size() == 0);

    delete xmlDoc;
    delete xslDoc;
}

int
main()
{
    testXslNormal();
    testXslNoTemplate();
    testXslEmptySelect();
    testXslOther();
    //testXslParam();

    return 0;
}


