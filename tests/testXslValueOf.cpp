

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
    test_assert(xmlDoc != nullptr);
    test_assert(xslDoc != nullptr);
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
            <text> hello <text/>
        </root>
    );

    std::string xsl = xml_code(
        <xsl:value-of select="text" />
    );

    Xml::Log log;
    auto xmlDoc = Xml::parse(xml);
    auto xslDoc = Xml::parse(xsl);

    test_assert(xmlDoc != nullptr);
    test_assert(xmlDoc != nullptr);
    test_assert(xslDoc != nullptr);
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
    test_assert(xmlDoc != nullptr);
    test_assert(xslDoc != nullptr);
    test_assert(xslDoc != nullptr);

    Xsl::ValueOf valueOf;

    Xml::Log transformLog;
    auto r = valueOf(xmlDoc->root(), *xslDoc, xslDoc->root(), transformLog);

    test_assert(r.size() == 0);


    delete xmlDoc;
    delete xslDoc;
}

void
textXslNonRecursive()
{
    std::string xml = xml_code(
        <root>
            <child>
                <text> hello <text/>
            </child>
        </root>
    );

    std::string xsl = xml_code(
        <xsl:value-of select="/" />
    );

    Xml::Log log;
    auto xmlDoc = Xml::parse(xml);
    auto xslDoc = Xml::parse(xsl);

    test_assert(xmlDoc != nullptr);
    test_assert(xmlDoc != nullptr);
    test_assert(xslDoc != nullptr);
    test_assert(xslDoc != nullptr);

    Xsl::ValueOf valueOf;

    Xml::Log transformLog;
    auto r = valueOf(xmlDoc->root(), *xslDoc, xslDoc->root(), transformLog);
    //std::cerr << ((Xml::Element *)r[0])->contentText() << std::endl;

    test_assert(((Xml::Element *)r[0])->contentText() == "hello");


    delete xmlDoc;
    delete xslDoc;
}


int
main()
{
    testXslEmptyValue();
    textXslHello();
    textXslNonExist();
    //textXslNonRecursive();

    return 0;
}
