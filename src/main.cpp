
#include "XmlParser.hpp"


int
main(int argc, char const * const * argv)
{
    if (argc != 2)
    {
        std::cerr << "syntaxe: " << argv[0] << " <xml_path>" << std::endl;
        return 1;
    }

    std::string xmlPath = argv[1];

    std::string * xmlDoc = Xml::parse(xmlPath);

    if (xmlDoc == 0)
    {
        std::cerr << "failed to parse XML file: " << xmlPath << std::endl;
        return 2;
    }

    delete xmlDoc;

    return 0;
}
