

/*
 * To test the Xml ValueOf, we need to access some protected/private field
 */
#define protected public
#define private public

#include "testhelper.hpp"

#include "../src/Xml/Xml.hpp"
#include "../src/Xsl/XslApplyTemplates.hpp"


void
testXslNoSelect()
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
          <xsl:apply-templates/>
    );

    Xml::Log log;
    auto xmlDoc = Xml::parse(xml);
    auto xslDoc = Xml::parse(xsl);

    test_assert(xmlDoc != nullptr);
    test_assert(xslDoc != nullptr);

    Xsl::ApplyTemplates applytemp;

    Xml::Log transformLog;
    auto r = applytemp(xmlDoc->root(), *xslDoc, xslDoc->root(), transformLog);
    //std::cerr << "))==>"<< ((Xml::Element *)r[0])->contentText() << std::endl;

    test_assert(r.size() == 12);
    test_assert(r[0]->contentText() == "Empire Burlesque");
    test_assert(r[1]->contentText() == "Bob Dylan");
    test_assert(r[6]->contentText() == "Hide your heart");

    delete xmlDoc;
    delete xslDoc;
}

void
testXslDefault()
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
        <xsl:apply-templates select="cd/title"/>
    );

    Xml::Log log;
    auto xmlDoc = Xml::parse(xml);
    auto xslDoc = Xml::parse(xsl);

    test_assert(xmlDoc != nullptr);
    test_assert(xslDoc != nullptr);

    Xsl::ApplyTemplates applytemp;

    Xml::Log transformLog;
    auto r = applytemp(xmlDoc->root(), *xslDoc, xslDoc->root(), transformLog);
    //std::cerr << "))==>"<< ((Xml::Element *)r[0])->contentText() << std::endl;

    test_assert(r.size() == 2);
    test_assert(r[0]->contentText() == "Empire Burlesque");
    test_assert(r[1]->contentText() == "Hide your heart");


    delete xmlDoc;
    delete xslDoc;
}

void
testXslNonExistSelect()
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
        <xsl:apply-templates select="title"/>
    );

    Xml::Log log;
    auto xmlDoc = Xml::parse(xml);
    auto xslDoc = Xml::parse(xsl);

    test_assert(xmlDoc != nullptr);
    test_assert(xslDoc != nullptr);

    Xsl::ApplyTemplates applytemp;

    Xml::Log transformLog;
    auto r = applytemp(xmlDoc->root(), *xslDoc, xslDoc->root(), transformLog);
    test_assert(r.size() == 0)
    delete xmlDoc;
    delete xslDoc;
}

int
main()
{
    testXslNoSelect();
    testXslDefault();
    testXslNonExistSelect();

    return 0;
}


