
#include <sstream>

#include "../AppDebug.hpp"

#include "Xsd.hpp"
#include "XsdElement.hpp"
#include "XsdValidate.hpp"


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

        for (auto instructionElement : xsdElement->elements())
        {
            if (instructionElement->namespaceName() != "xsd")
            {
                // TODO: log
                return false;
            }

            std::string instructionName = instructionElement->name();
            std::string instructionRegex;

            auto instruction = Xsd::instruction(instructionName);

            if (instruction == nullptr)
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
        Xml::Document const * xsdDocument,
        Xml::Element const * xsdElement
    ) const
    {
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
        s << name;
        s << ">";

        regexOut = s.str();

        return true;
    }

}
