
#include <mk_test.h>
#include "../src/XmlParser.hpp"


void
testHelperParseXml(std::string const & path, bool success = true)
{
    bool successed = false;

    std::string * e = Xml::parse(path);

    successed = ((e != 0) == success);

    if (successed)
    {
        test_passed(path.c_str());
    }
    else
    {
        test_failed(path.c_str());
    }

    if (e)
    {
        delete e;
    }
}


int
main()
{
    testHelperParseXml("tests/sample_01.xml");

    return 0;
}
