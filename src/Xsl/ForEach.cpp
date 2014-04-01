#include "ForEach.hpp"

std::vector<Xml::Node *>
Xsl::ForEach::operator () (const Xml::Element* context, const Xml::Document& xslDoc,  const Xml::Element * forEachElement) const
 {
    std::vector<Xml::Node*> resultNodes;
    auto matchingNodes = context->select(forEachElement->attribute("select"));
    for (auto node : matchingNodes) {
        for (auto resultNode : Xsl::applyTemplate(node, xslDoc, forEachElement)) {
            resultNodes.push_back(resultNode);
        }
    }

    return resultNodes;
}
