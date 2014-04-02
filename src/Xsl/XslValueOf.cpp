#include <string>
#include <vector>

#include "XslValueOf.hpp"

#include "../Xml/XmlDocument.hpp"
#include "../Xml/XmlElement.hpp"
#include "../Xml/XmlNode.hpp"
#include "../Xml/XmlText.hpp"

#include "../AppDebug.hpp"


std::vector<Xml::Node *>
Xsl::ValueOf::operator () (Xml::Element const * context,
    Xml::Document const &, Xml::Element const * xslElement, Xml::Log & xslLog) const
{
    app_assert(context != nullptr);
    app_assert(xslElement != nullptr);

    std::string select = xslElement->attribute("select");
    std::string value = context->valueOf(select);

    if (value == "")
    {
        return {};
    }

    return {new Xml::Text(value)};
}
