
#include <sstream>
#include <re2/re2.h>

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
        Xml::Element const * xsdElement,
        Xml::Log & log
    ) const
    {
        app_assert(xmlElement != nullptr);
        app_assert(xsdDocument != nullptr);
        app_assert(xsdElement != nullptr);
        app_assert(xsdElement->tag() == "xsd:element");

        auto xmlElementTheoricName = xsdElement->attribute("name");

        if (xmlElement->tag() != xmlElementTheoricName)
        {
            log.append("XSD: element <" + xmlElement->tag() + " /> mismatch (should have been <" + xmlElementTheoricName + "/>)" );
            return false;
        }

        auto elementContent = stringifyElement(xmlElement);
        std::ostringstream elementRegex;

        for (auto instructionElement : xsdElement->elements())
        {
            if (instructionElement->namespaceName() != "xsd")
            {
                log.append("XSD: unexpected element <" + instructionElement->tag() + "/>");
                return false;
            }

            std::string instructionName = instructionElement->name();
            auto instruction = Xsd::instruction(instructionName);

            if (instruction == nullptr)
            {
                log.append("XSD: unknown instruction <xsd:" + instructionName + "/>");
                return false;
            }

            std::string instructionRegex;

            elementRegex << instructionRegex;
        }

        if (!RE2::FullMatch(elementContent, elementRegex.str()))
        {
            log.append("XSD: element's children mismatch");
            return false;
        }

        // TODO: recusively check

        return true;
    }

    bool
    Element::regex(
        std::string & regexOut,
        Xml::Document const * xsdDocument,
        Xml::Element const * xsdElement,
        Xml::Log & log
    ) const
    {
        app_assert(xsdDocument != nullptr);
        app_assert(xsdElement != nullptr);
        app_assert(xsdElement->tag() == "xsd:element");

        std::ostringstream s;
        auto name = xsdElement->attribute("name");

        if (name == "")
        {
            log.append("XSD: missing attributes name");
            return false;
        }

        s << "<";
        s << name;
        s << ">";

        regexOut = s.str();

        return true;
    }

}
