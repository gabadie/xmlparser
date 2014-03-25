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
        virtual void operator () (Xml::Element * xslElement, Xml::Node* XmlContext);
    };

    std::map<std::string, Instruction*> xslInstructions;

    Xml::Document* xslTransform(Xml::Document& xmlDoc, Xml::Document& xslDoc ) ;

    void applyDefaultTemplate(const Xml::Node* context, const Xml::Document& xslDoc, vector< Xml::Node*> resultNodes) ;

    void applyTemplate (const Xml::Node* context, const Xml::Document& xslDoc, vector< Xml::Node*> resultNodes,   const Xml::Element& xslTemplate) ;
}
#endif //_H_XSL_XSL
