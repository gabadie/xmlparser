#include "ValueOf.hpp"

std::vector<Xml::Node *>
Xsl::ValueOf::operator () (const Xml::Element* context, const Xml::Document& xslDoc,  Xml::Element const * xslElement) const
{
    std::vector<Xml::Node*>  resultNodes;
    std::string resultText = xslElement->attribute("select");

    resultText = static_cast<const Xml::Element *>(context)->valueOf(resultText);
    Xml::Text * textValue = new Xml::Text(resultText);

    resultNodes.push_back(textValue);
    return resultNodes;
}
