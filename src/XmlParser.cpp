
#include <fstream>
#include <sstream>

#include "XmlParser.hpp"


Xml::Document *
Xml::load(std::string const & path, Xml::Log * log)
{
    std::ifstream f (path);

    if (!f.is_open())
    {
        return nullptr;
    }

    /*
     * Work around -> we put the entire xml content in a string first.
     */
    std::string xmlContent;

    f >> xmlContent;

    return Xml::parse(xmlContent, log);
}

Xml::Document *
Xml::parse(std::string const & xmlContent, Xml::Log * log)
{
    std::istringstream f (xmlContent);

    return Xml::parse(f, log);
}
