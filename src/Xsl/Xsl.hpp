#ifndef _H_XSL_XSL
#define _H_XSL_XSL

#include <vector>
#include <map>
using namespace std;

#include "../Xml/XmlNode.hpp"
#include "../Xml/XmlText.hpp"
#include "../Xml/XmlDocument.hpp"

namespace Xsl {

    class Instruction
    {
        virtual void operator () (Xml::Element * xslElement, Xml::Node* XmlContext) = 0;
    };


    class ValueOf : public Instruction
    {
        virtual void operator () (Xml::Node* context, const Xml::Document& xslDoc,  vector <Xml::Node*> &resultNodes, const Xml::Element xslElement);
    };

    class ForEach: public Instruction
    {
        virtual void operator () (Xml::Node* context,const Xml::Document& xslDoc,  vector <Xml::Node*> &resultNodes,  const Xml::Element forEachElement);
    };

    class ApplyTemplate: public Instruction
    {
        virtual void operator () (const Xml::Node* context,const Xml::Document& xslDoc, vector <Xml::Node*> &resultNodes, const Xml::Element applyTemplateElement);

    };

    Xml::Document* xslTransform(Xml::Document& xmlDoc, Xml::Document& xslDoc ) ;


    /**
     * Returns the appropriate template (if any) that should be applied
     * to the given element
     *
     * @param xslDoc The XSL document containing the templates
     * @param element The element that the returned template should be applied to
     *
     * @return the template that matches the given element. If no template matches, we return nullptr
     */
    const
    Xml::Element *
    getTemplate(Xml::Document& xslDoc, const Xml::Element* element);

    void applyDefaultTemplate( Xml::Node* context, Xml::Document& xslDoc, vector< Xml::Node*> &resultNodes) ;

    void applyTemplate ( Xml::Node* context, Xml::Document& xslDoc, vector< Xml::Node*> &resultNodes, Xml::Element& xslTemplate) ;
}
#endif //_H_XSL_XSL
