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
    // Debug only
    if (1==1)
    {
        resultNodes.push(context);
        return;
    }

    for (auto child : context.children())
    {
        Xsl::Element xslTemplate = this.getTemplate(child);

        if (xslTemplate == 0)
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
    for (Xml::Node* node : xslTemplate.children())
    {
        if (node->isElement() && node->namespace() == "xsl")
        {
            (*xslInstructions[((Xml::Element*) node)->name()])(context, node, resultNodes);
        }
        else
        {
            resultNodes.push(node);
        }
    }
}