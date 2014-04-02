
#include "XslForEach.hpp"
#include "XslTemplate.hpp"

#include "../Xml/XmlDocument.hpp"
#include "../Xml/XmlElement.hpp"
#include "../Xml/XmlNode.hpp"

#include "../AppDebug.hpp"


std::vector<Xml::Node *>
Xsl::ForEach::operator () (Xml::Element const * context,
    Xml::Document const & xslDoc, Xml::Element const * forEachElement, Xml::Log & xslLog) const
{
    app_assert(context != nullptr);
    app_assert(forEachElement != nullptr);

    std::vector<Xml::Node *> resultNodes;

    auto matchingNodes = context->select(forEachElement->attribute("select"));

    for (auto node : matchingNodes)
    {
        app_assert(node != nullptr);

        app_assert(node->isElement() == true);

        auto returnedNodes = Xsl::applyTemplate(static_cast<Xml::Element const *>(node), xslDoc, forEachElement, xslLog);

        resultNodes.reserve(resultNodes.size() + returnedNodes.size());

        for (auto resultNode : returnedNodes)
        {
            app_assert(resultNode != nullptr);

            resultNodes.push_back(resultNode);
        }
    }

    return resultNodes;
}
