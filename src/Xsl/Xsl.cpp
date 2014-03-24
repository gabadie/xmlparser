//penser à garder la racine qqpart
//demander si besoin de regex
#include <vector>
#include <map>


#include "./Xsl.hpp"

std::map<std::string, Xsl::Instruction*> xslInstructions;

void xslTransform(Xml::Document& xmlDoc, Xml::Document& xslDoc ) 
{

    Xml::Document result = new Xml::Document();
    vector <Xml::Node*> resultNodes;

    applyDefaultTemplate(xmlDoc.root(), resultNodes);

    if (resultNodes.size() == 1)
    {
        result.setRoot(resultNodes[0]);
        return result;
    }

    else 
    {
        throw Exception();
    }

}

void applyDefaultTemplate(const Xml::Node* context, const Xml::Document& xslDoc, vector<Node*> resultNodes) 
{
    if (1==1)
    {
        resultNodes.push(context);
        return;
    }

    for (auto child : context.children())
    {
        Xsl::Element xslTemplate = this.getTemplate(child);

        if (xslTemplate == null) 
        {
            applyDefaultTemplate(context, resultNodes);
            return ;
        }
        else
        {
           applyTemplate(xslTemplate, child, resultNodes);
           return ;
        }
    }
}

void applyTemplate (const Xml::Node* context, const Xml::Document& xslDoc, vector<Node*> resultNodes,   const Xml::Element& xslTemplate) 
{
    //To suppress ? Already in defaultTemplate
    if (xslTemplate == null) 
    {
        return applyDefaultTemplate(context, resultNodes);
    }

    // Attention, ici on parcours des éléments XSL, et pas le document XML qu'on transforme
    for (auto node : xslTemplate.children()) 
    {
        if (node.iselement() && node.namespace() != "xsl") 
        {
            resultNodes.push(node);
        }
        else 
        {
            xslInstructions[node->name()]->(context, node, resultNodes);
        }
    }
}

void Xsl::ValueOf::operator () (const Xml::Node* context, const Xml::Document& xslDoc,  vector <Xml::Node*> resultNodes, const Xml::Element xslElement)
{
   /* resultNodes.push(context.select(xslElement.attr("select").text()));*/
}

void Xsl::ForEach::operator () (const Xml::Node* context,const Xml::Document& xslDoc,  vector <Xml::Node*> resultNodes,  const Xml::Element forEachElement)
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