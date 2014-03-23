
#include <sstream>

#include "testhelper.hpp"

/*
 * To test the Xml parser, we need to access some protected/private field
 */
#define protected public
#define private public

#include "../src/Xml/XmlProcessingInstruction.hpp"
#include "../src/Xml/XmlParser.hpp"

void
testXmlPIAttributes()
{
    Xml::ProcessingInstruction xmlPI("xml", "version", "1.0", "encoding", "UTF-8");

    test_assert(xmlPI.mName == "xml");
    test_assert(xmlPI.attribute("version") == "1.0");
    test_assert(xmlPI.attribute("encoding") == "UTF-8");
}

void
testXmlPIExport()
{
    Xml::ProcessingInstruction xmlPI("xml", "version", "1.0", "encoding", "UTF-8");

    std::string pi = "<?xml encoding=\"UTF-8\" version=\"1.0\"?>";

    {
        std::ostringstream oss;
        xmlPI >> oss;
        test_assert(oss.str() == pi);
    }

    {
        std::ostringstream oss;
        oss << xmlPI;
        test_assert(oss.str() == pi);
    }

    {
        std::ostringstream oss;
        xmlPI.exportToStream(oss, 0, "");
        test_assert(oss.str() == pi);
    }
}

void
testXmlPIParsing()
{
    //TODO
}

int
main()
{
    testXmlPIAttributes();
    testXmlPIExport();
    testXmlPIParsing();

    return 0;
}
