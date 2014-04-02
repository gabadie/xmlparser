
#include <sstream>

#include "../AppDebug.hpp"

#include "XsdElement.hpp"


namespace Xsd
{

    bool
    Element::validate(
        Xml::Element const * xmlElement,
        Xml::Document const * xsdDocument,
        Xml::Element const * xsdElement
    ) const
    {
        app_assert(xmlElement != nullptr);
        app_assert(xsdDocument != nullptr);
        app_assert(xsdElement != nullptr);
        app_assert(xsdElement->tag() == "xsd:element");

        return false;
    }

    bool
    Element::regex(
        std::string & regexOut,
        Xml::Element const * xmlElement,
        Xml::Document const * xsdDocument,
        Xml::Element const * xsdElement
    ) const
    {
        app_assert(xmlElement != nullptr);
        app_assert(xsdDocument != nullptr);
        app_assert(xsdElement != nullptr);
        app_assert(xsdElement->tag() == "xsd:element");

        std::ostringstream s;

        for (auto child : xsdElement->elements())
        {
            std::string regex;

            /*if (!child->regex(regex, nullptr, xsdDocument, xsdElement))
            {
                return false;
            }*/

            (void)child;

            s << regex;
        }

        regexOut = s.str();

        return true;
    }

}
