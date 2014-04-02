
#include <sstream>

#include "Xsl.hpp"
#include "XslTemplate.hpp"

#include "../AppDebug.hpp"
#include "../Xml/XmlDocument.hpp"


Xml::Document *
Xsl::transform(Xml::Document const & xmlDoc, Xml::Document const & xslDoc, Xml::Log & xslLog)
{
    auto result = new Xml::Document();
    if (XslCheckValidity(xslDoc ,xslLog) != true)
    {
        xslLog.append("XSL format is not correct.");
        return result;
    }
    auto resultNodes = findAndApplyTemplate(xmlDoc.root(), xslDoc, xslLog);

    if (resultNodes.size() == 0)
    {
        xslLog.append("XSL transform returned empty document.");
        return result;
    }


    for (auto node : resultNodes)
    {
        app_assert(node != nullptr);

        if (node->isElement())
        {
            if (result->root() != nullptr)
            {
                auto element = static_cast<Xml::Element *>(node);

                std::ostringstream s;

                s << "XSL transform failed to create root element <";
                s << element->tag();
                s << " />: an other element <";
                s << result->root()->tag();
                s <<" /> has already been inserted in the document";

                xslLog.append(s.str());
                delete node;

                continue;
            }
        }
        else if (!Xml::Document::canAppend(node))
        {
            std::ostringstream s;

            s << "XSL transform has failed to append node ";
            s << *node;
            s << " into the document";

            xslLog.append(s.str());
            delete node;

            continue;
        }

        result->appendNode(node);
    }

    if (result->root() == nullptr)
    {
        xslLog.append("XSL transform has not created any root element");
    }

    return result;
}

bool 
Xsl::XslCheckValidity(Xml::Document const & xslDoc, Xml::Log & xslLog)
{
    app_assert(&xslDoc != nullptr);
    app_assert(&xslLog != nullptr);
    app_assert(xslDoc.root() != nullptr);

    if (xslDoc.root()->tag() != "xsl:stylesheet")
    {
        xslLog.append("Document is not a real Xsl. Tag stylesheet does not exist.");
        return false;
    }

    for ( auto rNodes : xslDoc.root()->elements())
    {
        if (rNodes->tag() != "xsl:template")
        {
            xslLog.append("Document is not a real Xsl. One of the children is not a template ");
            return false;
        }
    }
    return true;
}
