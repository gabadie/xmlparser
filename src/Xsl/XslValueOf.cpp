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
    Xml::Document const &, Xml::Element const * xslElement) const
{
    app_assert(context != nullptr);
    app_assert(xslElement != nullptr);

    std::vector<Xml::Node *> resultNodes;
    std::string resultText = xslElement->attribute("select");

    resultText = context->valueOf(resultText);
    Xml::Text * textValue = new Xml::Text(resultText);

    resultNodes.push_back(textValue);
    return resultNodes;
}
