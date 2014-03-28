#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <list>

#include "./Xsl.hpp"


std::map<std::string, Xsl::Instruction*> xslInstructions;

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
        result->setRoot((Xml::Element *) root);
    }
    catch(...)
    {
        throw ; //TODO ici logger tout ça ?
    }

}

Xml::Node * Xsl::applyDefaultTemplate( const Xml::Node * context,  Xml::Document& xslDoc, Xml::Document& xmlDoc)
{
    std::cerr << "coucou";
    if (!context->isElement())
    {
        return context->clone();
    }

    else
    {
        Xml::Element * resultElementNode = new Xml::Element(((Xml::Element *)context)->name());

        const Xml::Element * contextTemplate = getTemplate(xslDoc, (const Xml::Element *)context);

        // if the context has a template, possible the first time we come in this method
        if (contextTemplate)
        {
            for (Xml::Node* node : applyTemplate(context,xslDoc, contextTemplate, xmlDoc))
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
                Xml::Node* resultChild;

                //duplicate the test with the one present in Xsl::applydefaulttemplate
                const Xml::Element * xslTemplate = Xsl::getTemplate(xslDoc, (const Xml::Element *)child);
                if (xslTemplate == 0)
                {
                    resultChild = Xsl::applyDefaultTemplate((const Xml::Node *)child, xslDoc, xmlDoc);
                    resultElementNode->appendNode(resultChild);
                }
                else
                {
                    //we concatain all the results of the template to one Node.
                    vector<Xml::Node*> generatedChildren ;
                    generatedChildren = Xsl::applyTemplate(child, xslDoc, xslTemplate, xmlDoc);
                    for ( auto generatedChild : generatedChildren)
                    {
                        resultElementNode->appendNode(generatedChild);
                    }
                }
            }
        }

        return resultElementNode; //applyTemplate(context, xslDoc, xslTempla)  
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
        if (node->isElement() /*&& node->namespace() == "xsl"*/)
        {
            std::vector <Xml::Node *> resultInstruction = (*xslInstructions[((Xml::Element*) node)->name()])(context, xslDoc, (const Xml::Element *) node);
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

vector <Xml::Node*>  Xsl::ValueOf::operator () (const Xml::Node* context, Xml::Document& xslDoc, const Xml::Element * xslElement)
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

vector <Xml::Node*>  Xsl::ForEach::operator () (const Xml::Node* context, Xml::Document& xslDoc,  const Xml::Element * forEachElement)
 {
    Xml::Document * xmlDoc = new Xml::Document();//TODO
    list <Xml::Element const*> matchingNodes =((const Xml::Element *) context)->select(forEachElement->attribute("select"));
    for (auto node : matchingNodes) {
        Xsl::applyDefaultTemplate(node, xslDoc, *xmlDoc);
    }
}

 vector <Xml::Node*> Xsl::ApplyTemplate::operator () (const Xml::Node* context,Xml::Document& xslDoc, const Xml::Element * applyTemplateElement)
{
   /* Xml::Document * xmlDoc = new Xml::Document();//TODO

    vector <Xml::Node*> matchingNodes = ((const Xml::Element *) context)->select(applyTemplateElement->attribute("select"));
    for (auto node : matchingNodes){
        Xsl::Element const * xslTemplate = Xsl::getTemplate(xslDoc,  *(Element *) node) ;
        Xsl::applyTemplate(node, xslDoc, xslTemplate, xmlDoc)
    }*/
}