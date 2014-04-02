
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

int
main()
{
    testXslEmptyValue();

    return 0;
}
