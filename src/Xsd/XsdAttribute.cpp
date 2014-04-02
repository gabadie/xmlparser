#include "../AppDebug.hpp"

#include "XsdAttribute.hpp"

namespace Xsd
{

    bool
    Attribute::validate(
        Xml::Element const * xmlElement,
        Xml::Document const * xsdDocument,
        Xml::Element const * xsdElement,
        Xml::Log & log
    ) const
    {
        app_assert(xmlElement != nullptr);
        app_assert(xsdDocument != nullptr);
        app_assert(xsdElement != nullptr);
        app_assert(xsdElement->tag() == "xsd:attribute");

        bool valid = true;

        std::string attrName = xsdElement->attribute("name");

        if(attrName != "")
        {
            valid &= (xmlElement->attribute(attrName) != "");
        }

        return valid;
    }

    bool
    Attribute::regex(
        std::string & regexOut,
        Xml::Document const * xsdDocument,
        Xml::Element const * xsdElement,
        Xml::Log & log
    ) const
    {
        app_assert(xsdDocument != nullptr);
        app_assert(xsdElement != nullptr);
        app_assert(xsdElement->tag() == "xsd:attribute");

        regexOut = "";

        return true;
    }

}
