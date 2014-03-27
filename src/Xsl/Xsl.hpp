#ifndef _H_XSL_XSL
#define _H_XSL_XSL

#include <vector>
#include <map>
using namespace std;

#include "../Xml/XmlNode.hpp"
#include "../Xml/XmlText.hpp"
#include "../Xml/XmlDocument.hpp"
#include "../Xml/XmlForward.hpp"


typedef vector< Xml::Node*> nodeVector;

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

    Xml::Document * xslTransform(Xml::Document& xmlDoc, Xml::Document& xslDoc ) ;

}
#endif //_H_XSL_XSL
