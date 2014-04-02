#include "Xsd.hpp"

#include "../Xml/XmlElement.hpp"

#include "../AppDebug.hpp"

namespace Xsd
{

std::string
stringifyElement(Xml::Element const * element)
{
    app_assert(element != nullptr);

    std::string result = "";

    for(auto const e : element->elements())
    {
        result += "<" + e->tag() + ">";
    }

    return result;
}

}
