#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <list>
#include <string>

#include "./Xsl.hpp"
#include "../AppDebug.hpp"

std::map<std::string, Xsl::Instruction const*> const xslInstructions {
   { "apply-templates", (Xsl::Instruction*) new Xsl::ApplyTemplate() },
   { "for-each", (Xsl::Instruction*) new Xsl::ForEach() },
   { "value-of", (Xsl::Instruction*) new Xsl::ValueOf() }
};


bool deeperMatch(const Xml::Element* xslTemplateA, const Xml::Element* xslTemplateB) {
    std::string matchA = xslTemplateA->attribute("match");
    std::string matchB = xslTemplateB->attribute("match");
    return std::count(matchA.begin(), matchA.end(), '/') >= std::count(matchB.begin(), matchB.end(), '/') ;
}


const Xml::Element* Xsl::getTemplate(const Xml::Document& xslDoc, const Xml::Element* element) {
    const Xml::Element* curTemplate = nullptr;

    for (const Xml::Element* xslTemplate : xslDoc.root()->elements()) {
        // If the template has a "deeper match" than the currently selected template, we choose is instead
        if (element->matches(xslTemplate->attribute("match"))
            && (curTemplate == nullptr || deeperMatch(xslTemplate, curTemplate))) {
            curTemplate = xslTemplate;
        }
    }

    return curTemplate;
}


Xml::Document * Xsl::xslTransform( const Xml::Document& xmlDoc,  const Xml::Document& xslDoc)
{
    Xml::Document * result = new Xml::Document();
    std::vector<Xml::Node*> resultNodes = findAndApplyTemplate(xmlDoc.root(), xslDoc);

    if (resultNodes.size() == 0) {
        return result;
    }
    auto root = static_cast<Xml::Element *>(resultNodes[0]);
    result->setRoot(root);

    return result;
}


std::vector<Xml::Node *> Xsl::applyDefaultTemplate(Xml::Node const * context,  const Xml::Document& xslDoc)
{
    std::vector<Xml::Node *> result;
    if (!context->isElement())
    {
        result.push_back(context->clone());
        return result;
    }
    else
    {
        return findAndApplyTemplate(static_cast<const Xml::Element*>(context), xslDoc);
    }
}


std::vector<Xml::Node *> Xsl::findAndApplyTemplate(Xml::Element const * context, const Xml::Document& xslDoc) {
    const Xml::Element * contextTemplate = getTemplate(xslDoc, static_cast<const Xml::Element*>(context));

    // if the context a template matches the context, we apply it
    if (contextTemplate != nullptr)
    {
        return applyTemplate(context, xslDoc, contextTemplate);
    }
    else {
        std::vector<Xml::Node *> result;
        for (auto child: context->children()) {
            for (auto rNode : applyDefaultTemplate(child, xslDoc)) {
                result.push_back(rNode);
            }
        }
        return result;
    }
}


std::vector<Xml::Node *> Xsl::applyTemplate(Xml::Element const * context, const Xml::Document& xslDoc,  Xml::Element const * xslTemplate)
{
    std::vector<Xml::Node *> result;

    // Attention, ici on parcours des éléments XSL, et pas le document XML qu'on transforme
    for (Xml::Node* templateNode : xslTemplate->children())
    {
        if (!templateNode->isElement()) {
            result.push_back(templateNode->clone());
            continue;
        }

        auto templateElement = static_cast<const Xml::Element*>(templateNode);
        if (templateElement->namespaceName() == "xsl")
        {
            const Xml::Element* xslElement = templateElement;
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
            auto clonedElement = static_cast<Xml::Element*>(templateElement->clone());
            auto resultNodes = applyTemplate(static_cast<const Xml::Element*>(context), xslDoc, templateElement);

            for (auto rNode : resultNodes) {
                clonedElement->appendNode(rNode);
            }

            result.push_back(clonedElement);
        }
    }

    return result;

}


std::vector <Xml::Node*>  Xsl::ValueOf::operator () (const Xml::Element* context, const Xml::Document& xslDoc,  Xml::Element const * xslElement) const
{
    std::vector<Xml::Node*>  resultNodes;
    std::string resultText = xslElement->attribute("select");

    resultText = static_cast<const Xml::Element *>(context)->valueOf(resultText);
    Xml::Text * textValue = new Xml::Text(resultText);

    resultNodes.push_back(textValue);
    return resultNodes;
}


std::vector <Xml::Node*>  Xsl::ForEach::operator () (const Xml::Element* context, const Xml::Document& xslDoc,  const Xml::Element * forEachElement) const
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


std::vector <Xml::Node*> Xsl::ApplyTemplate::operator () (const Xml::Element* context,const Xml::Document& xslDoc,  Xml::Element const * applyTemplateElement) const
{
    std::vector <Xml::Node*> resultNodes ;
    std::list <Xml::Element const *> matchingNodes = context->select(applyTemplateElement->attribute("select"));

    for ( auto element : matchingNodes)
    {   
        for ( auto  appliedElement : findAndApplyTemplate(element, xslDoc))
        {
            resultNodes.push_back(appliedElement);
        }
    }

    return resultNodes;
}   