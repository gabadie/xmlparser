#include "ApplyTemplates.hpp"

#include "../Xml/XmlElement.hpp"
#include "../Xml/XmlNode.hpp"

std::vector<Xml::Node *>
Xsl::ApplyTemplates::operator () (Xml::Element const * context,
    Xml::Document const & xslDoc, Xml::Element const * applyTemplatesElement) const
{
    std::vector <Xml::Node*> resultNodes;

    if (applyTemplatesElement->attribute("select") == "")
    {
        for (auto element : context->elements())
        {
            for (auto appliedElement : findAndApplyTemplate(element, xslDoc))
            {
                resultNodes.push_back(appliedElement);
            }
        }
    }
    else
    {
        for (auto element : context->select(applyTemplatesElement->attribute("select")))
        {
            for (auto appliedElement : findAndApplyTemplate(element, xslDoc))
            {
                resultNodes.push_back(appliedElement);
            }
        }
    }

    return resultNodes;
}
