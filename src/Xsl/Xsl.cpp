//penser à garder la racine qqpart
//demander si besoin de regex
#include "Xsl.hpp"

std::map<std::string, Xsl::Instruction*> xslInstructions;

void xslTransform(Xml::Document& xmlDoc, Xml::Document& xslDoc ) 
{

    Xml::Document result = new Xml::Document();
    Vector <Xml::Node*> resultNodes;

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

void applyDefaultTemplate(Xml::Node* context, Vector<Node*> resultNodes) 
{
    // TODO : faire ça autrement...
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

void applyTemplate (const Xml::Element& xslTemplate, const Xml::Node* context, Vector<Node*> resultNodes) 
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

void Xsl::ValueOf::operator () (Xml::Node* context, Xml::Element xslElement, Vector <Xml::Node*> resultNodes)
{
    resultNodes.push(context.select(xslElement.attr("select").text()));
}