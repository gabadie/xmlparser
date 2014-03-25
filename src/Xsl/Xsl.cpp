 //penser à garder la racine qqpart
//demander si besoin de regex
#include <vector>
#include <map>
#include <iostream>

#include "./Xsl.hpp"

std::map<std::string, Xsl::Instruction*> xslInstructions;

Xml::Document* Xsl::xslTransform(Xml::Document& xmlDoc, Xml::Document& xslDoc )
{
    Xml::Document* result = new Xml::Document();

    Xml::Node* root = Xsl::applyDefaultTemplate(xmlDoc.root(), xslDoc);

    try
    {
        result->setRoot((Xml::Element*) root);
        return result;
    }
    catch(...)
    {
        throw; //A LOGGER
    }
}

Xml::Node* Xsl::applyDefaultTemplate(Xml::Node* context, Xml::Document& xslDoc)
{
    // Debug only
    if (!context->isElement())
    {
        return &();
    }

    // We create a new element
    Xml::Element* resultNode = new Xml::Element(context->name());

    // We generate its children
    for (auto child : ((Xml::Element*)context)->children())
    {
        Xml::Node* resultChild;

        Xml::Element xslTemplate = xslDoc.getTemplate(child);
        if (xslTemplate == 0)
        {
            resultChild = applyDefaultTemplate(child, xslDoc, resultNodes);
            resultNode->children().push_back(resultChild);
        }
        else
        {
           vector<Xml::Node*> generatedChildren = applyTemplate(child, xslDoc, resultChildren, xslTemplate);
           resultNode->setChildren( generatedChildren.end(), resultNode->children().begin(), resultNode->children().end() );
        }
    }

    return resultNode;
}


vector<Xml::Node*>  applyTemplate (Xml::Node* context, vector<Xml::Node*> resultNodes, Xml::Document& xslDoc, Xml::Element& xslTemplate)
{
    vector<Xml::Node*>  resultNodes;

    if (&xslTemplate == nullptr)
    {
        return Xsl::applyDefaultTemplate(context, xslDoc);
    }

    // Attention, ici on parcours des éléments XSL, et pas le document XML qu'on transforme
    for (Xml::Node* node : xslTemplate.children())
    {
        if (node->isElement() && node->namespace() == "xsl")
        {
            (*xslInstructions[((Xml::Element*) node)->name()])(context, xslDoc, resultNodes, node);
        }
        else
        {
            resultNodes.push_back(node);
        }
    }

}


void Xsl::ValueOf::operator () (Xml::Node* context, const Xml::Document& xslDoc,  vector <Xml::Node*> &resultNodes, const Xml::Element xslElement)
{
   /* resultNodes.push_back(context.select(xslElement.attr("select").text()));*/
}

void Xsl::ForEach::operator () (Xml::Node* context,const Xml::Document& xslDoc,  vector <Xml::Node*> &resultNodes,  const Xml::Element forEachElement)
 {
   /* vector <Xml::Node*> matchingNodes = context.select(forEachElement.attr('select'));
    for (auto node : matchingNodes) {
        applyTemplate(forEachElement, node, resultNodes);
    } */
}

 void Xsl::ApplyTemplate::operator () (const Xml::Node* context,const Xml::Document& xslDoc, vector <Xml::Node*> &resultNodes, const Xml::Element applyTemplateElement)
{
    /*vector <Xml::Node*> matchingNodes = context.select(forEachElement.attr('select'));
    for (auto node ; matchingNodes){
        Xsl::Element xslTemplate = DOCUMENT.getTemplate(node);
        applyTemplate(xslTemplate, node, resultNodes)
    }*/
}