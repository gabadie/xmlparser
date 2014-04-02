
#include <sstream>

#include "../AppDebug.hpp"

#include "Xsd.hpp"
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

        if (xmlElement->tag() != xsdElement->attribute("name"))
        {
            // TODO: log
            return false;
        }

        auto elementContent = stringifyElement(xmlElement);
        std::ostringstream elementRegex;

        for (auto instruction : xsdElement->elements())
        {
            if (instruction->namespaceName() != "xsd")
            {
                // TODO: log
                return false;
            }

            std::string instructionName = instruction->name();
            std::string instructionRegex;

            if (false)
            {
                // TODO log
                return false;
            }

            elementRegex << instructionRegex;
        }

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
        auto name = xsdElement->attribute("name");

        if (name == "")
        {
            // missing attribute name
            return false;
        }

        s << "<";
        s << xmlElement->tag();
        s << ">";

        regexOut = s.str();

        return true;
    }

}
