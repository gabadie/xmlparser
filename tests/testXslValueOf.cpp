

/*
 * To test the Xml ValueOf, we need to access some protected/private field
 */
#define protected public
#define private public

#include "testhelper.hpp"

#include "../src/Xml/Xml.hpp"
#include "../src/Xsl/XslValueOf.hpp"



void
testXslEmptyValue()
{
    std::string xml = xml_code(
        <root>
            <foo />
        </root>
    );

    std::string xsl = xml_code(
        <xsl:value-of select="" />
    );

    Xml::Log log;
    auto xmlDoc = Xml::parse(xml);
    auto xslDoc = Xml::parse(xsl);

    test_assert(xmlDoc != nullptr);
    test_assert(xslDoc != nullptr);

    Xsl::ValueOf valueOf;

    Xml::Log transformLog;
    auto r = valueOf(xmlDoc->root(), *xslDoc, xslDoc->root(), transformLog);

    test_assert(r.size() == 0);

    delete xmlDoc;
    delete xslDoc;
}


void
textXslHello()
{
    std::string xml = xml_code(
        <root>
            <text> hello </text>
        </root>
    );

    std::string xsl = xml_code(
        <xsl:value-of select="text" />
    );

    Xml::Log log;
    auto xmlDoc = Xml::parse(xml);
    auto xslDoc = Xml::parse(xsl);

    test_assert(xmlDoc != nullptr);
    test_assert(xslDoc != nullptr);

    Xsl::ValueOf valueOf;

    Xml::Log transformLog;
    auto r = valueOf(xmlDoc->root(), *xslDoc, xslDoc->root(), transformLog);

    test_assert(r[0]->contentText() == "hello");


    delete xmlDoc;
    delete xslDoc;
}

void
textXslNonExist()
{
    std::string xml = xml_code(
        <root>
            <text> hello <text/>
        </root>
    );

    std::string xsl = xml_code(
        <xsl:value-of select="/NonExist" />
    );

    Xml::Log log;
    auto xmlDoc = Xml::parse(xml);
    auto xslDoc = Xml::parse(xsl);

    test_assert(xmlDoc != nullptr);
    test_assert(xslDoc != nullptr);

    Xsl::ValueOf valueOf;

    Xml::Log transformLog;
    auto r = valueOf(xmlDoc->root(), *xslDoc, xslDoc->root(), transformLog);

    test_assert(r.size() == 0);


    delete xmlDoc;
    delete xslDoc;
}

void
textXslRecursive()
{
    std::string xml = xml_code(
        <root>
            <child>
                First Level
                <coui> Second level </coui>
                <text> Second level Bis </text>
            </child>
            <child>
                First Level
                <coui> Seconssd level </coui>
                <text> Second level Bis </text>
            </child>
        </root>
    );

    std::string xsl = xml_code(
        <xsl:value-of select="/child" />
    );

    Xml::Log log;
    auto xmlDoc = Xml::parse(xml);
    auto xslDoc = Xml::parse(xsl);

    test_assert(xmlDoc != nullptr);
    test_assert(xslDoc != nullptr);

    Xsl::ValueOf valueOf;

    Xml::Log transformLog;
    auto r = valueOf(xmlDoc->root(), *xslDoc, xslDoc->root(), transformLog);

    // std::cerr << ((Xml::Element *)r[0])->contentText() << std::endl;

    test_assert(r.size() == 1);

    test_assert(r[0]->contentText() == "First LevelSecond levelSecond level Bis");


    delete xmlDoc;
    delete xslDoc;
}


void
textXslAttributes()
{
    std::string xml = xml_code(
        <root>
            <child param1="First">
                First Level
                <coui param2="Second"> Second level </coui>
                <text param3="Third"> Second level Bis </text>
            </child>
        </root>
    );

    std::string xsl1 = xml_code(
        <xsl:value-of select="child/@param1" />
    );

    std::string xsl2 = xml_code(
        <xsl:value-of select="/child/coui/@param2" />
    );

    std::string xsl3 = xml_code(
        <xsl:value-of select="child/text/@param3" />
    );

    Xml::Log log;
    auto xmlDoc = Xml::parse(xml);
    auto xslDoc1 = Xml::parse(xsl1);
    auto xslDoc2 = Xml::parse(xsl2);
    auto xslDoc3 = Xml::parse(xsl3);

    test_assert(xmlDoc != nullptr);
    test_assert(xslDoc1 != nullptr);
    test_assert(xslDoc2 != nullptr);
    test_assert(xslDoc3 != nullptr);

    Xsl::ValueOf valueOf;

    Xml::Log transformLog;
    auto r1 = valueOf(xmlDoc->root(), *xslDoc1, xslDoc1->root(), transformLog);
    auto r2 = valueOf(xmlDoc->root(), *xslDoc2, xslDoc2->root(), transformLog);
    auto r3 = valueOf(xmlDoc->root(), *xslDoc3, xslDoc3->root(), transformLog);

    test_assert(r1[0]->contentText() == "First");
    test_assert(r2[0]->contentText() == "Second");
    test_assert(r3[0]->contentText() == "Third");


    delete xmlDoc;
    delete xslDoc1;
    delete xslDoc2;
    delete xslDoc3;
}


int
main()
{
    testXslEmptyValue();
    textXslHello();
    textXslNonExist();
    textXslRecursive();
    textXslAttributes();

    return 0;
}
