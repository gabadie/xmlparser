#include "../AppDebug.hpp"

#include "XsdAttribute.hpp"

namespace Xsd
{

    bool
    Attribute::validate(
        Xml::Element const * xmlElement,
        Xml::Document const * xsdDocument,
        Xml::Element const * xsdElement
    ) const
    {
        app_assert(xmlElement != nullptr);
        app_assert(xsdDocument != nullptr);
        app_assert(xsdElement != nullptr);
        app_assert(xsdElement->tag() == "xsd:attribute");

        return false;
    }

    bool
    Attribute::regex(
        std::string & regexOut,
        Xml::Element const * xmlElement,
        Xml::Document const * xsdDocument,
        Xml::Element const * xsdElement
    ) const
    {
        app_assert(xmlElement != nullptr);
        app_assert(xsdDocument != nullptr);
        app_assert(xsdElement != nullptr);
        app_assert(xsdElement->tag() == "xsd:attribute");

        regexOut = "";

        return false;
    }

}
