
#include "XmlParser.hpp"


int
main(int argc, char const * const * argv)
{
    if (argc != 2 || argv[1] == 0)
    {
        std::cerr << "syntaxe: " << argv[0] << " <xml_path>" << std::endl;
        return 1;
    }

    Xml::Document * xmlDoc = Xml::load(std::string(argv[1]));

    if (xmlDoc == 0)
    {
        std::cerr << "failed to parse XML file: " << argv[1] << std::endl;
        return 2;
    }

    delete xmlDoc;

    return 0;
}
