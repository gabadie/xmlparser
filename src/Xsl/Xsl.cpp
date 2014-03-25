//penser à garder la racine qqpart
//demander si besoin de regex
#include <vector>
#include <map>


#include "./Xsl.hpp"
using namespace Xsl;

Xml::Document* xslTransform(Xml::Document& xmlDoc, Xml::Document& xslDoc )
{

    Xml::Document* result = new Xml::Document();
    vector <Xml::Node*> resultNodes;

    applyDefaultTemplate(xmlDoc.root(), xslDoc, resultNodes);

    try
    {
        result->setRoot((Xml::Element*) resultNodes[0]);
        return result;
    }
    catch(...)
    {
        throw; //A LOGGER 
    }

}

void applyDefaultTemplate(Xml::Node* context, Xml::Document& xslDoc, vector<Xml::Node*> resultNodes) 
{

    // Debug only

    if (1==1)
    {
        resultNodes.push_back(context);
        return;
    }

    for (auto child : ((Xml::Element*)context)->children())
    {
        /*Xml::Element xslTemplate = xslDoc.getTemplate(child);

        if (xslTemplate == 0)
        {
            applyDefaultTemplate(context, xslDoc, resultNodes);
            return ;
        }
        else
        {
           applyTemplate(child, xslDoc, resultNodes, xslTemplate);
           return ;
        }*/
            applyDefaultTemplate(context, xslDoc, resultNodes);
    }
}

void applyTemplate (Xml::Node* context, const Xml::Document& xslDoc, vector<Xml::Node*> resultNodes,    Xml::Element& xslTemplate) 
{
    //To suppress ? Already in defaultTemplate
    if (&xslTemplate == nullptr    )
    {
        return applyDefaultTemplate(context, xslDoc, resultNodes);
    }

    // Attention, ici on parcours des éléments XSL, et pas le document XML qu'on transforme
    for (Xml::Node* node : xslTemplate.children())
    {
       /* if (node->isElement() && node->namespace() == "xsl")
        {
            (*xslInstructions[((Xml::Element*) node)->name()])(context, xslDoc, resultNodes, node);
        }
        else
        {*/
            resultNodes.push_back(node);
        /*}*/
    }

}

void Xsl::ValueOf::operator () (Xml::Node* context, const Xml::Document& xslDoc,  vector <Xml::Node*> resultNodes, const Xml::Element xslElement)
{
   /* resultNodes.push_back(context.select(xslElement.attr("select").text()));*/
}

void Xsl::ForEach::operator () (Xml::Node* context,const Xml::Document& xslDoc,  vector <Xml::Node*> resultNodes,  const Xml::Element forEachElement)
 {
   /* vector <Xml::Node*> matchingNodes = context.select(forEachElement.attr('select'));
    for (auto node : matchingNodes) {
        applyTemplate(forEachElement, node, resultNodes);
    } */
}

 void Xsl::ApplyTemplate::operator () (const Xml::Node* context,const Xml::Document& xslDoc, vector <Xml::Node*> resultNodes, const Xml::Element applyTemplateElement) 
{
    /*vector <Xml::Node*> matchingNodes = context.select(forEachElement.attr('select'));
    for (auto node ; matchingNodes){
        Xsl::Element xslTemplate = DOCUMENT.getTemplate(node);
        applyTemplate(xslTemplate, node, resultNodes)
    }*/
}