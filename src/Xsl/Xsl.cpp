#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <list>
#include <string>

#include "./Xsl.hpp"
#include "../AppDebug.hpp"

std::map<std::string, Xsl::Instruction*> const xslInstructions {
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
    std::vector<Xml::Node*> resultNodes = findAndApplyTemplate(xmlDoc.root(), xslDoc);
    app_assert(resultNodes.size() == 1);
    app_assert(resultNodes[0]->isElement());
    auto root = static_cast<Xml::Element *>(resultNodes[0]);

    Xml::Document * result = new Xml::Document();
    result->setRoot(root);

    return result;
}

Xml::Node * Xsl::applyDefaultTemplate(const Xml::Node * context,  const Xml::Document& xslDoc)
{
    if (!context->isElement())
    {
        return context->clone();
    }
    else
    {
        Xml::Element * resultElementNode = static_cast<Xml::Element*>(context->clone());
        for (auto rNode : findAndApplyTemplate(static_cast<const Xml::Element*>(context), xslDoc)) {
            resultElementNode->appendNode(rNode);
        }
        return resultElementNode;
    }
}

std::vector<Xml::Node *> Xsl::findAndApplyTemplate(const Xml::Element * context, const Xml::Document& xslDoc) {
    const Xml::Element * contextTemplate = getTemplate(xslDoc, static_cast<const Xml::Element*>(context));

    // if the context a template matches the context, we apply it
    if (contextTemplate != nullptr)
    {
        return applyTemplate(context, xslDoc, contextTemplate);
    }
    else {
        std::vector<Xml::Node *> resultNodes;
        resultNodes.push_back(applyDefaultTemplate(context, xslDoc));
        return resultNodes;
    }
}

std::vector<Xml::Node *> Xsl::applyTemplate(const Xml::Element * context, const Xml::Document& xslDoc, const Xml::Element * xslTemplate)
{
    std::vector<Xml::Node *> listNodes;

    // Attention, ici on parcours des éléments XSL, et pas le document XML qu'on transforme
    for (Xml::Node* templateNode : xslTemplate->children())
    {
        if (!templateNode->isElement()) {
            listNodes.push_back(templateNode->clone());
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
                listNodes.push_back(node);
            }
        }
        else
        {
            auto clonedElement = static_cast<Xml::Element*>(templateElement->clone());
            auto resultNodes = applyTemplate(static_cast<const Xml::Element*>(context), xslDoc, templateElement);
            for (auto rNode : resultNodes) {
                clonedElement->appendNode(rNode);
            }
            listNodes.push_back(clonedElement);
        }
    }
    return listNodes;

}

std::vector <Xml::Node*>  Xsl::ValueOf::operator () (const Xml::Element* context, const Xml::Document& xslDoc, const Xml::Element * xslElement) const
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

std::vector <Xml::Node*> Xsl::ApplyTemplate::operator () (const Xml::Element* context,const Xml::Document& xslDoc, const Xml::Element * applyTemplateElement) const
{
    auto matchingNodes = context->select(applyTemplateElement->attribute("select"));
    for (auto node : matchingNodes){
        Xml::Element const * xslTemplate = Xsl::getTemplate(xslDoc, node) ;
        Xsl::applyTemplate(node, xslDoc, xslTemplate);
    }
}