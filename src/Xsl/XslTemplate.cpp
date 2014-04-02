#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "ApplyTemplates.hpp"

#include "XslForEach.hpp"
#include "XslInstruction.hpp"
#include "XslTemplate.hpp"
#include "XslValueOf.hpp"

#include "../Xml/XmlDocument.hpp"
#include "../Xml/XmlElement.hpp"
#include "../Xml/XmlNode.hpp"
#include "../Xml/XmlText.hpp"

#include "../AppDebug.hpp"

namespace
{

    /**
     * TODO: doc
     */
    std::map<std::string, Xsl::Instruction const *> const xslInstructions
    {
       { "apply-templates", new Xsl::ApplyTemplates() },
       { "for-each", new Xsl::ForEach() },
       { "value-of", new Xsl::ValueOf() }
    };

    /**
     * TODO: doc
     */
    bool
    deeperMatch(Xml::Element const * xslTemplateA, Xml::Element const * xslTemplateB)
    {
        std::string matchA = xslTemplateA->attribute("match");
        std::string matchB = xslTemplateB->attribute("match");
        return std::count(matchA.begin(), matchA.end(), '/') >= std::count(matchB.begin(), matchB.end(), '/') ;
    }
} // Anonymous namespace


Xml::Element const *
Xsl::getTemplate(Xml::Document const & xslDoc, Xml::Element const * element)
{
    Xml::Element const * curTemplate = nullptr;

    for (Xml::Element const * xslTemplate : xslDoc.root()->elements())
    {
        // If the template has a "deeper match" than the currently selected template, we choose is instead
        if (element->matches(xslTemplate->attribute("match")) &&
            (curTemplate == nullptr || deeperMatch(xslTemplate, curTemplate)))
        {
            curTemplate = xslTemplate;
        }
    }

    return curTemplate;
}

std::vector<Xml::Node *>
Xsl::applyDefaultTemplate(Xml::Node const * context, Xml::Document const & xslDoc)
{
    std::vector<Xml::Node *> result;
    if (!context->isElement())
    {
        result.push_back(context->clone());
        return result;
    }
    else
    {
        return findAndApplyTemplate(static_cast<Xml::Element const *>(context), xslDoc);
    }
}

std::vector<Xml::Node *>
Xsl::findAndApplyTemplate(Xml::Element const * context, Xml::Document const & xslDoc)
{
    Xml::Element const * contextTemplate = getTemplate(xslDoc, context);
    // if the context a template matches the context, we apply it
    if (contextTemplate != nullptr)
    {
        return applyTemplate(context, xslDoc, contextTemplate);
    }
    else
    {
        std::vector<Xml::Node *> result;
        for (auto child : context->children())
        {
            for (auto rNode : applyDefaultTemplate(child, xslDoc))
            {
                result.push_back(rNode);
            }
        }
        return result;
    }
}

std::vector<Xml::Node *>
Xsl::applyTemplate(Xml::Element const * context, Xml::Document const & xslDoc, Xml::Element const * xslTemplate)
{
    std::vector<Xml::Node *> result;

    // Attention, ici on parcourt des éléments XSL, et pas le document XML qu'on transforme
    for (Xml::Node * templateNode : xslTemplate->children())
    {
        if (!templateNode->isElement())
        {
            result.push_back(templateNode->clone());
            continue;
        }

        auto templateElement = static_cast<Xml::Element const *>(templateNode);
        if (templateElement->namespaceName() == "xsl")
        {
            Xml::Element const * xslElement = templateElement;
            auto instructionPair = xslInstructions.find(xslElement->name());

            if (instructionPair == xslInstructions.end()) {
                // Le tag XSL comprends une instruction inconnue
                // TODO : logger
                std::cerr << "instruction inconnue ! " << xslElement->name() << std::endl;
                continue;
            }

            Xsl::Instruction const * xslInstruction = instructionPair->second;

            std::vector <Xml::Node *> resultInstruction = (*xslInstruction)(context, xslDoc, xslElement);
            for (auto node : resultInstruction)
            {
                result.push_back(node);
            }
        }
        else
        {
            auto clonedElement = static_cast<Xml::Element *>(templateElement->clone());
            auto resultNodes = applyTemplate(context, xslDoc, templateElement);

            for (auto rNode : resultNodes) {
                clonedElement->appendNode(rNode);
            }

            result.push_back(clonedElement);
        }
    }

    return result;
}
