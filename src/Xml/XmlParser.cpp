
#include <fstream>
#include <sstream>

#include "XmlParser.hpp"


#include "../MemoryLeakTrackerOn.hpp"

Xml::Document *
Xml::load(std::string const & path, Xml::Log * log)
{
    std::ifstream f (path, std::ios::binary | std::ios::in);

    if (!f.is_open())
    {
        return nullptr;
    }

    /*
     * Work around -> we put the entire xml content in a string first.
     */
    std::string xmlContent((std::istreambuf_iterator<char>(f)),
                 std::istreambuf_iterator<char>());

    return Xml::parse(xmlContent, log);
}

Xml::Document *
Xml::parse(std::string const & xmlContent, Xml::Log * log)
{
    std::istringstream f (xmlContent);

    return Xml::parse(f, log);
}

#include "../MemoryLeakTrackerOff.hpp"
