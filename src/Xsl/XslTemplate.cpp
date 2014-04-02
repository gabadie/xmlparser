#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "XslApplyTemplates.hpp"
#include "XslForEach.hpp"
#include "XslInstruction.hpp"
#include "XslTemplate.hpp"
#include "XslValueOf.hpp"

#include "../Xml/XmlDocument.hpp"
#include "../Xml/XmlElement.hpp"
#include "../Xml/XmlForward.hpp"
#include "../Xml/XmlNode.hpp"
#include "../Xml/XmlText.hpp"

#include "../AppDebug.hpp"

namespace
{
    /**
     * The map of supported XSL instructions. Keys are the names of the instruction's XML tag,
     * value is a functor that applies the given template.
     */
    std::map<std::string, Xsl::Instruction const *> const xslInstructions
    {
       { "apply-templates", new Xsl::ApplyTemplates() },
       { "for-each", new Xsl::ForEach() },
       { "value-of", new Xsl::ValueOf() }
    };

    /**
     * Says which template has a deeper, more "specific", match attribute.
     * i.e : "cd/title" is more specific than "title".
     *
     * @param xslTemplateA first template element that's being compared
     * @param xslTemplateB second template element that's being compared
     *
     * @return true if xslTemplateA has a deeper or equal match than xslTemplateB, false otherwise
     */
    bool
    deeperMatch(Xml::Element const * xslTemplateA, Xml::Element const * xslTemplateB)
    {
        app_assert(xslTemplateA != nullptr);
        app_assert(xslTemplateB != nullptr);

        std::string matchA = xslTemplateA->attribute("match");
        std::string matchB = xslTemplateB->attribute("match");
        return std::count(matchA.begin(), matchA.end(), '/') >= std::count(matchB.begin(), matchB.end(), '/') ;
    }

} // Anonymous namespace

std::vector<Xml::Node *>
Xsl::applyDefaultTemplate(Xml::Node const * context, Xml::Document const & xslDoc, Xml::Log & xslLog)
{
    app_assert(context != nullptr);

    std::vector<Xml::Node *> result;

    Xml::ObjectLabel label = context->objectLabel();
    if (label == Xml::ObjectLabel::Text || label == Xml::ObjectLabel::CharacterData ||
        label == Xml::ObjectLabel::ProcessingInstruction)
    {
        result.push_back(context->clone());
    }
    else
    {
        result = findAndApplyTemplate(static_cast<Xml::Element const *>(context), xslDoc, xslLog);
    }

    return result;
}

Xml::Element const *
Xsl::getTemplate(Xml::Document const & xslDoc, Xml::Element const * element)
{
    app_assert(element != nullptr);

    Xml::Element const * curTemplate = nullptr;

    for (Xml::Element const * xslTemplate : xslDoc.root()->elements())
    {
        app_assert(xslTemplate != nullptr);

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
Xsl::findAndApplyTemplate(Xml::Element const * context, Xml::Document const & xslDoc, Xml::Log & xslLog)
{
    app_assert(context != nullptr);

    Xml::Element const * contextTemplate = getTemplate(xslDoc, context);

    // if the context a template matches the context, we apply it
    if (contextTemplate != nullptr)
    {
        return applyTemplate(context, xslDoc, contextTemplate, xslLog);
    }
    else
    {
        std::vector<Xml::Node *> result;
        for (auto child : context->children())
        {
            app_assert(child != nullptr);

            for (auto rNode : applyDefaultTemplate(child, xslDoc, xslLog))
            {
                app_assert(rNode != nullptr);
                result.push_back(rNode);
            }
        }
        return result;
    }
}


std::vector<Xml::Node *>
Xsl::applyTemplate(Xml::Element const * context, Xml::Document const & xslDoc, Xml::Element const * xslTemplate, Xml::Log & xslLog)
{
    app_assert(context != nullptr);
    app_assert(xslTemplate != nullptr);

    std::vector<Xml::Node *> result;

    // We iterate over the XSL template's children
    for (Xml::Node * templateNode : xslTemplate->children())
    {
        app_assert(templateNode != nullptr);

        if (!templateNode->isElement())
        {
            Xml::ObjectLabel label = templateNode->objectLabel();
            if (label == Xml::ObjectLabel::Text || label == Xml::ObjectLabel::CharacterData)
            {
                result.push_back(templateNode->clone());
            }
            continue;
        }

        auto templateElement = static_cast<Xml::Element const *>(templateNode);
        if (templateElement->namespaceName() == "xsl")
        {
            Xml::Element const * xslElement = templateElement;
            auto instructionPair = xslInstructions.find(xslElement->name());

            if (instructionPair == xslInstructions.end()) {
                // Unknown XSL instruction
                xslLog.append("Unknown XSL instruction: " + xslElement->name() + "\n");
                continue;
            }

            Xsl::Instruction const * xslInstruction = instructionPair->second;

            app_assert(xslInstruction != nullptr);

            std::vector <Xml::Node *> resultInstruction = (*xslInstruction)(context, xslDoc, xslElement, xslLog);
            for (auto node : resultInstruction)
            {
                app_assert(node != nullptr);
                result.push_back(node);
            }
        }
        else
        {
            auto clonedElement = static_cast<Xml::Element *>(templateElement->clone());
            auto resultNodes = applyTemplate(context, xslDoc, templateElement, xslLog);

            app_assert(clonedElement != nullptr);

            for (auto rNode : resultNodes) {
                app_assert(rNode != nullptr);
                clonedElement->appendNode(rNode);
            }

            result.push_back(clonedElement);
        }
    }

    return result;
}
