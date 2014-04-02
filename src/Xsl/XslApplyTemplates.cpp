#include "XslApplyTemplates.hpp"
#include "XslTemplate.hpp"

#include "../Xml/XmlElement.hpp"
#include "../Xml/XmlNode.hpp"

#include "../AppDebug.hpp"

std::vector<Xml::Node *>
Xsl::ApplyTemplates::operator () (Xml::Element const * context,
    Xml::Document const & xslDoc, Xml::Element const * applyTemplatesElement, Xml::Log & xslLog) const
{
    app_assert(context != nullptr);
    app_assert(applyTemplatesElement != nullptr);

    std::vector <Xml::Node*> resultNodes;

    if (applyTemplatesElement->attribute("select") == "")
    {
        for (auto element : context->elements())
        {
            app_assert(element != nullptr);

            for (auto appliedElement : findAndApplyTemplate(element, xslDoc, xslLog))
            {
                app_assert(appliedElement != nullptr);
                resultNodes.push_back(appliedElement);
            }
        }
    }
    else
    {
        for (auto element : context->select(applyTemplatesElement->attribute("select")))
        {
            app_assert(element != nullptr);

            for (auto appliedElement : findAndApplyTemplate(element, xslDoc, xslLog))
            {
                app_assert(appliedElement != nullptr);
                resultNodes.push_back(appliedElement);
            }
        }
    }

    return resultNodes;
}
