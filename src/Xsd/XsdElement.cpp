
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

    std::string
    Element::regex(
        Xml::Element const * xmlElement,
        Xml::Document const * xsdDocument,
        Xml::Element const * xsdElement
    ) const
    {
        app_assert(xmlElement != nullptr);
        app_assert(xsdDocument != nullptr);
        app_assert(xsdElement != nullptr);
        app_assert(xsdElement->tag() == "xsd:element");

        // TODO CANTENOT
        return "";
    }

}
