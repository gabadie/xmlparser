#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <list>
#include <string>

#include "./Xsl.hpp"

std::map<std::string, Xsl::Instruction*> const xslInstructions {
   { "apply-templates", (Xsl::Instruction*) new Xsl::ApplyTemplate() },
   { "for-each", (Xsl::Instruction*) new Xsl::ForEach() },
   { "value-of", (Xsl::Instruction*) new Xsl::ValueOf() }
};

bool deeperMatch(const Xml::Element* xslTemplateA, const Xml::Element* xslTemplateB) {
    std::string matchA = xslTemplateA->attribute("match");
    std::string matchB = xslTemplateB->attribute("match");
    return std::count(matchA.begin(), matchA.end(), '/') > std::count(matchB.begin(), matchB.end(), '/') ;
}

const Xml::Element* Xsl::getTemplate(Xml::Document& xslDoc, const Xml::Element* element) {
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


Xml::Document * Xsl::xslTransform( Xml::Document& xmlDoc,  Xml::Document& xslDoc)
{
    Xml::Node * root = Xsl::applyDefaultTemplate(xmlDoc.root(), xslDoc);
    Xml::Document * result = new Xml::Document();
    try
    {
        result->setRoot(static_cast<Xml::Element*>(root));
    }
    catch(...)
    {
        throw ; //TODO ici logger tout ça ?
    }

    return result;
}

Xml::Node * Xsl::applyDefaultTemplate( const Xml::Node * context,  Xml::Document& xslDoc)
{

    if (!context->isElement())
    {
        return context->clone();
    }
    else
    {
        const Xml::Element * contextTemplate = getTemplate(xslDoc, static_cast<const Xml::Element*>(context));
        Xml::Element * resultElementNode = dynamic_cast<Xml::Element*>(context->clone());

        // if the context has a template, possible the first time we come in this method
        if (contextTemplate != nullptr)
        {
            for (Xml::Node* node : applyTemplate(dynamic_cast<const Xml::Element*>(context), xslDoc, contextTemplate))
            {
                resultElementNode->appendNode(node);
            }
            return resultElementNode;
        }
        else
        {
            // We generate its children
            for (auto child : (dynamic_cast<const Xml::Element*>(context))->children())
            {
                resultElementNode->appendNode(applyDefaultTemplate(child, xslDoc));
            }
        }

        return resultElementNode;
    }
}

std::vector<Xml::Node *> Xsl::applyTemplate(const Xml::Element * context, Xml::Document& xslDoc, const Xml::Element * xslTemplate)
{
    std::vector<Xml::Node * > listNodes;

    if (&xslTemplate == nullptr)
    {

        listNodes.push_back(Xsl::applyDefaultTemplate(context, xslDoc));
        return listNodes;
    }

    // Attention, ici on parcours des éléments XSL, et pas le document XML qu'on transforme
    for (Xml::Node* templateNode : xslTemplate->children())
    {
        if (templateNode->isElement() && ((Xml::Element*) templateNode)->namespaceName() == "xsl")
        {
            const Xml::Element* xslElement = static_cast<const Xml::Element *>(templateNode);
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
            listNodes.push_back(templateNode->clone());
        }
    }
    return listNodes;

}

vector <Xml::Node*>  Xsl::ValueOf::operator () (const Xml::Element* context, Xml::Document& xslDoc, const Xml::Element * xslElement) const
{

    std::cerr << "value of applied " << std::endl;
    vector <Xml::Node*>  resultNodes;
    std::string resultText;
    resultText = xslElement->attribute("select");
    std::cerr << "=>" << resultText << std::endl;

    resultText =((const Xml::Element *) context)->valueOf(resultText);
    Xml::Text * textValue = new Xml::Text(resultText);


    resultNodes.push_back(textValue);
    return resultNodes;
}

vector <Xml::Node*>  Xsl::ForEach::operator () (const Xml::Element* context, Xml::Document& xslDoc,  const Xml::Element * forEachElement) const
 {
    vector <Xml::Node*>  resultNodes;

    list <Xml::Element const*> matchingNodes = context->select(forEachElement->attribute("select"));
    for (auto node : matchingNodes) {
        for (auto resultNode : Xsl::applyTemplate(node, xslDoc, forEachElement)) {
            resultNodes.push_back(resultNode);
        }
    }

    return resultNodes;
}

 vector <Xml::Node*> Xsl::ApplyTemplate::operator () (const Xml::Element* context,Xml::Document& xslDoc, const Xml::Element * applyTemplateElement) const
{
    list <Xml::Element const *> matchingNodes = context->select(applyTemplateElement->attribute("select"));
    for (auto node : matchingNodes){
        Xml::Element const * xslTemplate = Xsl::getTemplate(xslDoc, node) ;
        Xsl::applyTemplate(node, xslDoc, xslTemplate);
    }
}