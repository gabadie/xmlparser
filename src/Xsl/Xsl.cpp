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
        return context->clone();
    }

    // We create a new element
    Xml::Element* resultNode = new Xml::Element(((Xml::Element *)context)->name());

    // We generate its children
    for (auto child : ((Xml::Element*)context)->children())
    {
        Xml::Node* resultChild;

        //duplicate the test with the one present in Xsl::applydefaulttemplate
        Xml::Element * xslTemplate = xslDoc.getTemplate(child);
        if (xslTemplate == 0)
        {
            resultChild = Xsl::applyDefaultTemplate(child, xslDoc);
            resultNode->appendNode(resultChild);
        }
        else
        {
            //we concatain all the results of the template to one Node.
            vector<Xml::Node*> generatedChildren ;
            Xsl::applyTemplate(child, xslDoc, generatedChildren, xslTemplate);
            for ( auto generatedChild : generatedChildren)
            {
                resultNode->appendNode(generatedChild);
            }
        }
    }

    return resultNode;
}


void Xsl::applyTemplate (Xml::Node* context,Xml::Document& xslDoc, vector<Xml::Node*> &listNodes,  Xml::Element * xslTemplate)
{

    if (&xslTemplate == nullptr)
    {
        listNodes.push_back(Xsl::applyDefaultTemplate(context, xslDoc));
        return;
    }

    // Attention, ici on parcours des éléments XSL, et pas le document XML qu'on transforme
    for (Xml::Node* node : xslTemplate->children())
    {
        if (node->isElement() /*&& node->namespace() == "xsl"*/)
        {
            listNodes.push_back((*xslInstructions[((Xml::Element*) node)->name()])(context, xslDoc, ((Xml::Element *)node)->children(), node));
        }
        else
        {
            listNodes.push_back(node->clone());
        }
    }
    return;

}


void Xsl::ValueOf::operator () (Xml::Node* context, const Xml::Document& xslDoc,  vector <Xml::Node*> &listNodes, const Xml::Element xslElement)
{
   /* resultNodes.push_back(context.select(xslElement.attr("select").text()));*/
}

void Xsl::ForEach::operator () (Xml::Node* context,const Xml::Document& xslDoc,  vector <Xml::Node*> &resultNodes,  const Xml::Element forEachElement)
 {
   /* vector <Xml::Node*> matchingNodes = context.select(forEachElement.attr('select'));
    for (auto node : matchingNodes) {
        Xsl::applyTemplate(forEachElement, node, resultNodes);
    } */
}

 void Xsl::Xsl::ApplyTemplate::operator () (const Xml::Node* context,const Xml::Document& xslDoc, vector <Xml::Node*> &resultNodes, const Xml::Element Xsl::applyTemplateElement)
{
    /*vector <Xml::Node*> matchingNodes = context.select(forEachElement.attr('select'));
    for (auto node ; matchingNodes){
        Xsl::Element xslTemplate = DOCUMENT.getTemplate(node);
        Xsl::applyTemplate(xslTemplate, node, resultNodes)
    }*/
}