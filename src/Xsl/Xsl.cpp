#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <list>

#include "./Xsl.hpp"

std::map<std::string, Xsl::Instruction*> const xslInstructions {
   { "apply-templates", (Xsl::Instruction*) new Xsl::ApplyTemplate() },
   { "for-each", (Xsl::Instruction*) new Xsl::ForEach() }
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
    Xml::Document * result = new Xml::Document();

    Xml::Node * root = Xsl::applyDefaultTemplate(xmlDoc.root(), xslDoc, xmlDoc);

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

Xml::Node * Xsl::applyDefaultTemplate( const Xml::Node * context,  Xml::Document& xslDoc, Xml::Document& xmlDoc)
{

    if (!context->isElement())
    {
        return context->clone();
    }

    else
    {
        Xml::Element * resultElementNode = (Xml::Element*) context->clone();

        const Xml::Element * contextTemplate = getTemplate(xslDoc, resultElementNode);

        // if the context has a template, possible the first time we come in this method
        if (contextTemplate != nullptr)
        {
            for (Xml::Node* node : applyTemplate(resultElementNode, xslDoc, contextTemplate, xmlDoc))
            {
                resultElementNode->appendNode(node);
            }
            return resultElementNode;
        }
        else
        {
            // We generate its children
            for (auto child : ((Xml::Element*)context)->children())
            {
                resultElementNode->appendNode(applyDefaultTemplate(child, xslDoc, xmlDoc));
            }
        }

        return resultElementNode;
    }
}

std::vector<Xml::Node *> Xsl::applyTemplate(const Xml::Node * context, Xml::Document& xslDoc,const Xml::Element * xslTemplate,  Xml::Document& xmlDoc)
{
    std::vector<Xml::Node * > listNodes;

    if (&xslTemplate == nullptr)
    {

        listNodes.push_back(Xsl::applyDefaultTemplate(context, xslDoc, xmlDoc));
        return listNodes;
    }

    // Attention, ici on parcours des éléments XSL, et pas le document XML qu'on transforme
    for (Xml::Node* node : xslTemplate->children())
    {
        if (node->isElement() && ((Xml::Element*) node)->namespaceName() == "xsl")
       {
            Xml::Element* xslElement = (Xml::Element*) node;
            auto instructionPair = xslInstructions.find(xslElement->name());

            if (instructionPair == xslInstructions.end()) {
                // Le tag XSL comprends une instruction inconnue
                // TODO : logger
                continue;
            }

            Xsl::Instruction const * xslInstruction = instructionPair->second;

            std::vector <Xml::Node *> resultInstruction = (*xslInstruction)(context, xslDoc, (const Xml::Element *) node);
            for (auto node : resultInstruction)
            {
                listNodes.push_back(node);
            }
        }
        else
        {
            listNodes.push_back(node->clone());
        }
    }
    return listNodes;

}

vector <Xml::Node*>  Xsl::ValueOf::operator () (const Xml::Node* context, Xml::Document& xslDoc, const Xml::Element * xslElement) const
{
   vector <Xml::Node*>  resultNodes;
   list <Xml::Element const*>  resultNodesTemp;

   resultNodesTemp = ((const Xml::Element *) context)->select(xslElement->attribute("select"));
   for(auto node : resultNodesTemp)
   {
        resultNodes.push_back(node->clone());
   }
    return resultNodes;
}

vector <Xml::Node*>  Xsl::ForEach::operator () (const Xml::Node* context, Xml::Document& xslDoc,  const Xml::Element * forEachElement) const
 {
    Xml::Document * xmlDoc = new Xml::Document();//TODO
    list <Xml::Element const*> matchingNodes =((const Xml::Element *) context)->select(forEachElement->attribute("select"));
    for (auto node : matchingNodes) {
        Xsl::applyDefaultTemplate(node, xslDoc, *xmlDoc);
    }
}

 vector <Xml::Node*> Xsl::ApplyTemplate::operator () (const Xml::Node* context,Xml::Document& xslDoc, const Xml::Element * applyTemplateElement) const
{
    Xml::Document * xmlDoc = new Xml::Document();//TODO

    list <Xml::Element const *> matchingNodes = ((const Xml::Element *) context)->select(applyTemplateElement->attribute("select"));
    for (auto node : matchingNodes){
        Xml::Element const * xslTemplate = Xsl::getTemplate(xslDoc,  node) ;
        Xsl::applyTemplate(node, xslDoc, xslTemplate, *xmlDoc);
    }
}