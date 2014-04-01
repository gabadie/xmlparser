#include "ForEach.hpp"

#include "../Xml/XmlDocument.hpp"
#include "../Xml/XmlElement.hpp"
#include "../Xml/XmlNode.hpp"

std::vector<Xml::Node *>
Xsl::ForEach::operator () (Xml::Element const * context,
    Xml::Document const & xslDoc, Xml::Element const * forEachElement) const
{
    std::vector<Xml::Node *> resultNodes;
    auto matchingNodes = context->select(forEachElement->attribute("select"));
    for (auto node : matchingNodes)
    {
        for (auto resultNode : Xsl::applyTemplate(node, xslDoc, forEachElement))
        {
            resultNodes.push_back(resultNode);
        }
    }

    return resultNodes;
}
