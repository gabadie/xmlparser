#include <mk_test.h>

#include "../src/Xml/XmlParser.hpp"
#include "../src/Xsl/Xsl.hpp"

#define xml_code(code) \
    ((const char *) #code)
void
testXslTransform()
{
    // xml
    std::string xmlContent (xml_code(
        <root>
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

    delete xmlDoc;
    delete xslDoc;
}

int
main()
{
    return 0;
}
