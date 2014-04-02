
#include "XslForEach.hpp"
#include "XslTemplate.hpp"

#include "../Xml/XmlDocument.hpp"
#include "../Xml/XmlElement.hpp"
#include "../Xml/XmlNode.hpp"

#include "../AppDebug.hpp"


std::vector<Xml::Node *>
Xsl::ForEach::operator () (Xml::Element const * context,
    Xml::Document const & xslDoc, Xml::Element const * forEachElement) const
{
    app_assert(context != nullptr);
    app_assert(forEachElement != nullptr);

    std::vector<Xml::Node *> resultNodes;
    auto matchingNodes = context->select(forEachElement->attribute("select"));
    for (auto node : matchingNodes)
    {
        app_assert(node != nullptr);

        for (auto resultNode : Xsl::applyTemplate(node, xslDoc, forEachElement))
        {
            app_assert(resultNode != nullptr);
            resultNodes.push_back(resultNode);
        }
    }

    return resultNodes;
}
